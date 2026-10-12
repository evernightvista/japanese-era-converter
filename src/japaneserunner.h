#ifndef JAPANESEERARUNNER_H
#define JAPANESEERARUNNER_H

#include <KRunner/AbstractRunner>

/**
 * @brief KRunner 日本年号转换器插件
 *
 * 支持双向转换：
 * - 输入公历年份（1910–2099）→ 显示对应的日本年号；
 * - 输入年号年数（如 "令和8年" / "Reiwa 8"）→ 显示对应的公历年份（如 2026）。
 *
 * 输入非有效内容时不显示任何结果。点击结果将文本复制到剪贴板：
 * 正向复制年号显示文本，反向复制公历年份。
 */
class JapaneseEraRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    explicit JapaneseEraRunner(QObject *parent, const KPluginMetaData &data);
    ~JapaneseEraRunner() override;

    /**
     * @brief 匹配用户输入
     *
     * - 纯数字（1910–2099）→ 正向转换为日本年号；
     * - 年号年数（如 "令和8年" / "Reiwa 8"）→ 反向转换为公历年份。
     * 无法识别的输入不添加任何匹配（KRunner 中不显示本插件）。
     */
    void match(KRunner::RunnerContext &context) override;

    /**
     * @brief 运行选中的匹配项
     *
     * 将转换结果复制到剪贴板。
     */
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;
};

#endif // JAPANESEERARUNNER_H
