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
    addSyntax(KRunner::RunnerSyntax(
        QStringLiteral(":q:"),
        i18n("Converts the Gregorian year :q: (1910-2099) to the Japanese era")));
}

JapaneseEraRunner::~JapaneseEraRunner() = default;

void JapaneseEraRunner::match(KRunner::RunnerContext &context)
{
    const QString query = context.query().trimmed();

    // 仅接受纯数字输入
    bool ok = false;
    const int year = query.toInt(&ok);
    if (!ok) {
        return;
    }

    // 仅接受有效年份范围 1910–2099，否则不显示
    if (!JapaneseEra::isValidYear(year)) {
        return;
    }

    const QVector<EraResult> results = JapaneseEra::convert(year);
    if (results.isEmpty()) {
        return;
    }

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
