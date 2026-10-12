#include "japaneseera.h"

#include <QString>

namespace {
/**
 * @brief 从年号名之后的字符串中解析年号年数
 *
 * 支持的形式：
 *   - "元年"        → 1
 *   - "8年" / "8"   → 8
 *   - " 8 "        → 8（去除空白与可选的结尾 "年"）
 * @return 解析出的年数（≥1）；无法解析时返回 -1
 */
int parseEraYear(const QString &s)
{
    QString t = s.trimmed();
    if (t.isEmpty()) {
        return -1;
    }

    // "元年" → 1
    if (t == QStringLiteral("元年")) {
        return 1;
    }

    // 去掉结尾的 "年"
    if (t.endsWith(QStringLiteral("年"))) {
        t.chop(1);
        t = t.trimmed();
    }

    if (t.isEmpty()) {
        return -1;
    }

    bool ok = false;
    const int year = t.toInt(&ok);
    if (!ok || year < 1) {
        return -1;
    }
    return year;
}

/**
 * @brief 年号汉字对应的汉语拼音（小写、连写，无空格/无声调）
 *
 * 用于支持在 KRunner 中以拼音输入年号名，例如 "zhaohe8年" → 昭和8年。
 * 五个年号：明治/大正/昭和/平成/令和。
 * @return 拼音字符串；无映射时返回空
 */
QString pinyinForEra(const QString &eraName)
{
    if (eraName == QStringLiteral("明治")) return QStringLiteral("mingzhi");
    if (eraName == QStringLiteral("大正")) return QStringLiteral("dazheng");
    if (eraName == QStringLiteral("昭和")) return QStringLiteral("zhaohe");
    if (eraName == QStringLiteral("平成")) return QStringLiteral("pingcheng");
    if (eraName == QStringLiteral("令和")) return QStringLiteral("linghe");
    return {};
}
} // namespace

QVector<Era> JapaneseEra::eras()
{
    // 基于日本官方改元日期
    // 来源：日本总务省/内阁府公布的元号一覧
    static const QVector<Era> s_eras = {
        // 明治：1868-10-23 ～ 1912-07-29（明治天皇崩御）
        { QStringLiteral("明治"), QStringLiteral("Meiji"),
          1868, 10, 23, 1912, 7, 29 },
        // 大正：1912-07-30 ～ 1926-12-24（大正天皇崩御）
        { QStringLiteral("大正"), QStringLiteral("Taisho"),
          1912, 7, 30, 1926, 12, 24 },
        // 昭和：1926-12-25 ～ 1989-01-07（昭和天皇崩御）
        { QStringLiteral("昭和"), QStringLiteral("Showa"),
          1926, 12, 25, 1989, 1, 7 },
        // 平成：1989-01-08 ～ 2019-04-30（天皇退位）
        { QStringLiteral("平成"), QStringLiteral("Heisei"),
          1989, 1, 8, 2019, 4, 30 },
        // 令和：2019-05-01 ～ 现在
        { QStringLiteral("令和"), QStringLiteral("Reiwa"),
          2019, 5, 1, 9999, 12, 31 },
    };
    return s_eras;
}

bool JapaneseEra::isValidYear(int year)
{
    return year >= MinYear && year <= MaxYear;
}

QVector<EraResult> JapaneseEra::convert(int gregorianYear)
{
    QVector<EraResult> results;

    if (!isValidYear(gregorianYear)) {
        return results;
    }

    const QVector<Era> eraList = eras();
    for (const Era &era : eraList) {
        // 判断该公历年是否与该年号有重叠
        // 年号覆盖区间：[startYear/startMonth/startDay, endYear/endMonth/endDay]
        // 对于仅输入年份的情况，只要该年有任意一天落在年号区间内即算重叠

        bool overlaps = false;

        if (gregorianYear > era.startYear && gregorianYear < era.endYear) {
            // 年份完全在年号区间内
            overlaps = true;
        } else if (gregorianYear == era.startYear && gregorianYear == era.endYear) {
            // 年号在同一年内开始并结束（极端情况，本数据不会出现）
            overlaps = true;
        } else if (gregorianYear == era.startYear) {
            // 改元年：从改元日到年末都属于该年号
            overlaps = true;
        } else if (gregorianYear == era.endYear) {
            // 终年：从年初到终年日都属于该年号
            overlaps = true;
        }

        if (!overlaps) {
            continue;
        }

        // 计算年号年数：年号元年 = 改元当年，第N年 = 公历 - 改元年 + 1
        const int eraYear = gregorianYear - era.startYear + 1;

        EraResult r;
        r.eraName = era.name;
        r.eraRomaji = era.romaji;
        r.eraYear = eraYear;

        // 元年特殊显示："元年" 而非 "1年"
        const QString yearText = (eraYear == 1)
            ? QStringLiteral("元年")
            : QString::number(eraYear) + QStringLiteral("年");

        r.display = QStringLiteral("%1%2 (%3 %4)")
            .arg(era.name, yearText, era.romaji, QString::number(eraYear));

        results.append(r);
    }

    return results;
}

QVector<ReverseEraResult> JapaneseEra::convertReverse(const QString &eraName, int eraYear)
{
    QVector<ReverseEraResult> results;

    if (eraYear < 1 || eraName.isEmpty()) {
        return results;
    }

    const QVector<Era> eraList = eras();
    for (const Era &era : eraList) {
        // 匹配年号名：汉字精确匹配 / 罗马字大小写不敏感 / 拼音大小写不敏感
        const bool nameMatch = (era.name == eraName)
            || (era.romaji.toLower() == eraName.toLower())
            || (pinyinForEra(era.name).toLower() == eraName.toLower());
        if (!nameMatch) {
            continue;
        }

        // 校验年数不超过该年号的实际跨度（已结束的年号）
        if (era.endYear != 9999) {
            const int eraMaxYear = era.endYear - era.startYear + 1;
            if (eraYear > eraMaxYear) {
                return results; // 年数超出该年号实际跨度
            }
        }

        // 计算公历年：元年 = 改元当年，第N年 = 改元年 + N - 1
        const int gregorianYear = era.startYear + eraYear - 1;

        // 限定在插件支持的有效范围内 [MinYear, MaxYear]
        if (gregorianYear < MinYear || gregorianYear > MaxYear) {
            return results;
        }

        ReverseEraResult r;
        r.eraName = era.name;
        r.eraRomaji = era.romaji;
        r.eraYear = eraYear;
        r.gregorianYear = gregorianYear;

        // 元年特殊显示："元年" 而非 "1年"
        const QString yearText = (eraYear == 1)
            ? QStringLiteral("元年")
            : QString::number(eraYear) + QStringLiteral("年");

        r.display = QStringLiteral("%1年 (%2%3 / %4 %5)")
            .arg(QString::number(gregorianYear),
                 era.name, yearText,
                 era.romaji, QString::number(eraYear));

        results.append(r);
        return results; // 匹配到唯一年号，直接返回
    }

    return results; // 未匹配到任何年号
}

QVector<ReverseEraResult> JapaneseEra::convertReverseFromQuery(const QString &rawQuery)
{
    const QString query = rawQuery.trimmed();
    if (query.isEmpty()) {
        return {};
    }

    const QVector<Era> eraList = eras();
    for (const Era &era : eraList) {
        // 1) 汉字年号名前缀匹配（大小写敏感，汉字无需折叠大小写）
        if (query.startsWith(era.name, Qt::CaseSensitive)) {
            const QString remainder = query.mid(era.name.length());
            const int eraYear = parseEraYear(remainder);
            if (eraYear > 0) {
                return convertReverse(era.name, eraYear);
            }
        }

        // 2) 罗马字年号名前缀匹配（大小写不敏感），支持 "Reiwa 8" / "reiwa8" 等
        if (query.startsWith(era.romaji, Qt::CaseInsensitive)) {
            const QString remainder = query.mid(era.romaji.length());
            const int eraYear = parseEraYear(remainder);
            if (eraYear > 0) {
                return convertReverse(era.name, eraYear);
            }
        }

        // 3) 拼音年号名前缀匹配（大小写不敏感），支持 "zhaohe8年" / "ZhaoHe8" 等
        const QString pinyin = pinyinForEra(era.name);
        if (!pinyin.isEmpty() && query.startsWith(pinyin, Qt::CaseInsensitive)) {
            const QString remainder = query.mid(pinyin.length());
            const int eraYear = parseEraYear(remainder);
            if (eraYear > 0) {
                return convertReverse(era.name, eraYear);
            }
        }
    }

    return {};
}
