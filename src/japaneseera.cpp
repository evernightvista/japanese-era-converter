#include "japaneseera.h"

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
