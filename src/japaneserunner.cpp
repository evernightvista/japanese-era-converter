#include "japaneserunner.h"
#include "japaneseera.h"

#include <KRunner/QueryMatch>
#include <KRunner/RunnerContext>
#include <KRunner/RunnerSyntax>
#include <KLocalizedString>

#include <QGuiApplication>
#include <QClipboard>
#include <QIcon>

JapaneseEraRunner::JapaneseEraRunner(QObject *parent, const KPluginMetaData &data)
    : KRunner::AbstractRunner(parent, data)
{
    setObjectName(QStringLiteral("Japanese Era Converter"));
    // 公历 → 年号
    addSyntax(KRunner::RunnerSyntax(
        QStringLiteral(":q:"),
        i18n("Converts the Gregorian year :q: (1910-2099) to the Japanese era")));
    // 年号 → 公历（反向）
    addSyntax(KRunner::RunnerSyntax(
        QStringLiteral("令和:q:"),
        i18n("Converts a Japanese era year (e.g. 令和8年 / Reiwa 8) to the Gregorian year")));
}

JapaneseEraRunner::~JapaneseEraRunner() = default;

void JapaneseEraRunner::match(KRunner::RunnerContext &context)
{
    const QString query = context.query().trimmed();
    if (query.isEmpty()) {
        return;
    }

    // 1) 纯数字 → 公历转年号（原功能）
    bool ok = false;
    const int year = query.toInt(&ok);
    if (ok) {
        // 仅接受有效年份范围 1910–2099，否则不显示
        if (!JapaneseEra::isValidYear(year)) {
            return;
        }

        const QVector<EraResult> results = JapaneseEra::convert(year);
        for (const EraResult &r : results) {
            KRunner::QueryMatch match(this);
            // 有效年份精确匹配，给予较高相关度
            match.setRelevance(0.95);
            match.setIcon(QIcon::fromTheme(QStringLiteral("view-calendar-japanese-era")));
            match.setText(r.display);
            match.setSubtext(i18n("Gregorian year %1 → click to copy to clipboard", year));
            // 用 data 存储完整显示文本，供 run 时复制
            match.setData(r.display);
            context.addMatch(match);
        }
        return;
    }

    // 2) 年号年数 → 公历（反向转换），支持汉字与罗马字输入
    const QVector<ReverseEraResult> revResults = JapaneseEra::convertReverseFromQuery(query);
    for (const ReverseEraResult &r : revResults) {
        KRunner::QueryMatch match(this);
        match.setRelevance(0.93);
        match.setIcon(QIcon::fromTheme(QStringLiteral("view-calendar-japanese-era")));
        match.setText(r.display);
        match.setSubtext(i18n("%1%2 → click to copy Gregorian year to clipboard",
                              r.eraName,
                              (r.eraYear == 1) ? QStringLiteral("元年")
                                               : QString::number(r.eraYear) + QStringLiteral("年")));
        // 反向转换复制公历年份，便于粘贴使用
        match.setData(QString::number(r.gregorianYear));
        context.addMatch(match);
    }
}

void JapaneseEraRunner::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match)
{
    Q_UNUSED(context)
    const QString text = match.data().toString();
    if (!text.isEmpty()) {
        QGuiApplication::clipboard()->setText(text);
    }
}

K_PLUGIN_CLASS_WITH_JSON(JapaneseEraRunner, "japaneseera.json")

#include "japaneserunner.moc"
