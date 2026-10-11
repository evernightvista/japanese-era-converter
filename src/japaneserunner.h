#ifndef JAPANESEERARUNNER_H
#define JAPANESEERARUNNER_H

#include <KRunner/AbstractRunner>

/**
 * @brief KRunner 日本年号转换器插件
 *
 * 在 KRunner 中输入公历年份（1910–2099），即可显示对应的日本年号。
 * 输入非有效年份时不显示任何结果。
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
     * 仅当输入为 1910–2099 之间的整数年份时才生成匹配结果，
     * 否则不添加任何匹配（KRunner 中不显示本插件）。
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
