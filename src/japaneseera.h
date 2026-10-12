#ifndef JAPANESEERA_H
#define JAPANESEERA_H

#include <QString>
#include <QVector>

/**
 * @brief 日本年号（元号）数据结构
 *
 * 基于日本官方公布的改元日期：
 * - 明治  1868-10-23 ～ 1912-07-29
 * - 大正  1912-07-30 ～ 1926-12-24
 * - 昭和  1926-12-25 ～ 1989-01-07
 * - 平成  1989-01-08 ～ 2019-04-30
 * - 令和  2019-05-01 ～ 现在
 */
struct Era {
    QString name;       ///< 汉字名称，如 "令和"
    QString romaji;     ///< 罗马字，如 "Reiwa"
    int startYear;      ///< 改元年（公历）
    int startMonth;     ///< 改元月
    int startDay;       ///< 改元日
    int endYear;        ///< 结束年（公历），当前年号用 9999 表示
    int endMonth;       ///< 结束月
    int endDay;         ///< 结束日
};

/**
 * @brief 单次转换结果
 */
struct EraResult {
    QString eraName;        ///< 年号汉字，如 "令和"
    QString eraRomaji;      ///< 年号罗马字，如 "Reiwa"
    int eraYear;            ///< 年号年数，如 9（令和9年）
    QString display;        ///< 完整显示文本，如 "令和9年 (Reiwa 9)"
};

/**
 * @brief 反向转换结果（年号年数 → 公历年）
 */
struct ReverseEraResult {
    QString eraName;        ///< 年号汉字，如 "令和"
    QString eraRomaji;      ///< 年号罗马字，如 "Reiwa"
    int eraYear;            ///< 年号年数，如 8（令和8年）
    int gregorianYear;      ///< 对应的公历年，如 2026
    QString display;        ///< 完整显示文本，如 "2026年 (令和8年 / Reiwa 8)"
};

class JapaneseEra
{
public:
    /**
     * @brief 获取覆盖 1910–2099 年的全部年号列表
     * @return 年号列表，按时间升序
     */
    static QVector<Era> eras();

    /**
     * @brief 将公历年份转换为日本年号
     *
     * 对于改元重叠的年份（如 1912、1926、1989、2019），
     * 会返回该年涉及的所有年号及对应年数。
     *
     * @param gregorianYear 公历年份（1910–2099）
     * @return 转换结果列表；若年份不在有效范围则返回空列表
     */
    static QVector<EraResult> convert(int gregorianYear);

    /**
     * @brief 将日本年号年数转换为公历年份（反向转换）
     *
     * 年号名支持三种形式：
     *   - 汉字（如 "令和"）
     *   - 罗马字（如 "Reiwa"，大小写不敏感）
     *   - 拼音（如 "linghe"，大小写不敏感）
     * 已结束的年号要求年数不超过其实际跨度（如 昭和 1–64）；
     * 当前年号（令和）由插件上限 MaxYear 约束。
     * 计算出的公历年必须在 [MinYear, MaxYear] 范围内。
     *
     * @param eraName 年号名（汉字 / 罗马字 / 拼音）
     * @param eraYear 年号年数（1 表示元年，必须 ≥ 1）
     * @return 转换结果列表（至多一项）；若年号名或年数无效则返回空
     */
    static QVector<ReverseEraResult> convertReverse(const QString &eraName, int eraYear);

    /**
     * @brief 从 KRunner 查询字符串解析并执行反向转换
     *
     * 支持的输入形式（以令和8年为例）：
     *   - 汉字：  "令和8年" / "令和8" / "令和元年" / "令和1年"
     *   - 罗马字："Reiwa 8" / "Reiwa8" / "reiwa8"（大小写不敏感）
     *   - 拼音：  "linghe8年" / "linghe8" / "LingHe8"（大小写不敏感）
     * 五个年号（明治/大正/昭和/平成/令和）均支持，对应拼音：
     *   mingzhi / dazheng / zhaohe / pingcheng / linghe
     *
     * @param query KRunner 输入文本
     * @return 转换结果列表；若无法识别为年号年数则返回空
     */
    static QVector<ReverseEraResult> convertReverseFromQuery(const QString &query);

    /**
     * @brief 判断年份是否在有效范围内
     */
    static bool isValidYear(int year);

    /// 有效年份下限
    static constexpr int MinYear = 1910;
    /// 有效年份上限
    static constexpr int MaxYear = 2099;
};

#endif // JAPANESEERA_H
