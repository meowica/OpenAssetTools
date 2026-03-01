#include "MenuExpressionMatchers.h"

#include "MenuMatcherFactory.h"
#include "Parsing/Menu/Domain/Expression/CommonExpressionBaseFunctionCall.h"
#include "Parsing/Menu/Domain/Expression/CommonExpressionCustomFunctionCall.h"

using namespace menu;

static constexpr int TAG_EXPRESSION_FUNCTION_CALL = SimpleExpressionMatchers::TAG_OFFSET_EXPRESSION_EXT + 1;
static constexpr int TAG_EXPRESSION_FUNCTION_CALL_END = SimpleExpressionMatchers::TAG_OFFSET_EXPRESSION_EXT + 2;

static constexpr int CAPTURE_FUNCTION_NAME = SimpleExpressionMatchers::CAPTURE_OFFSET_EXPRESSION_EXT + 1;

MenuExpressionMatchers::MenuExpressionMatchers(const MenuFileParserState* state)
    : SimpleExpressionMatchers(true, true, true, true, true),
      m_state(state)
{
}

MenuExpressionMatchers::MenuExpressionMatchers()
    : MenuExpressionMatchers(nullptr)
{
}

std::unique_ptr<SimpleExpressionMatchers::matcher_t> MenuExpressionMatchers::ParseOperandExtension(const supplier_t* labelSupplier) const
{
    const MenuMatcherFactory create(labelSupplier);

    return create.Or({
        create
            .And({
                create.Identifier().Capture(CAPTURE_FUNCTION_NAME),
                create.Char('('),
                create.Optional(create.And({
                    create.Label(LABEL_EXPRESSION),
                    create.OptionalLoop(create.And({
                        create.Char(','),
                        create.Label(LABEL_EXPRESSION),
                    })),
                })),
                create.Char(')').Tag(TAG_EXPRESSION_FUNCTION_CALL_END),
            })
            .Tag(TAG_EXPRESSION_FUNCTION_CALL),
    });
}

const std::map<std::string, size_t>& MenuExpressionMatchers::GetBaseFunctionMapForFeatureLevel(const FeatureLevel featureLevel)
{
    assert(false);
    throw ParsingException(TokenPos(), "Feature level has no functions registered!!");
}

std::unique_ptr<ISimpleExpression> MenuExpressionMatchers::ProcessOperandExtension(SequenceResult<SimpleParserValue>& result) const
{
    assert(m_state);
    if (m_state == nullptr)
        throw ParsingException(TokenPos(), "No state when processing menu operand extension!!");

    if (result.PeekAndRemoveIfTag(TAG_EXPRESSION_FUNCTION_CALL) != TAG_EXPRESSION_FUNCTION_CALL)
        throw ParsingException(TokenPos(), "Menu Operand Extension must be function call");

    const auto& functionCallToken = result.NextCapture(CAPTURE_FUNCTION_NAME);
    auto functionCallName = functionCallToken.IdentifierValue();
    utils::MakeStringLowerCase(functionCallName);

    const auto& baseFunctionMap = GetBaseFunctionMapForFeatureLevel(m_state->m_feature_level);
    const auto foundBaseFunction = baseFunctionMap.find(functionCallName);
    if (foundBaseFunction != baseFunctionMap.end())
    {
        auto functionCall = std::make_unique<CommonExpressionBaseFunctionCall>(std::move(functionCallName), foundBaseFunction->second);
        while (result.PeekAndRemoveIfTag(TAG_EXPRESSION_FUNCTION_CALL_END) != TAG_EXPRESSION_FUNCTION_CALL_END)
        {
            functionCall->m_args.emplace_back(ProcessExpression(result));
        }
        return std::move(functionCall);
    }

    const auto foundCustomFunction = m_state->m_functions_by_name.find(functionCallName);
    if (foundCustomFunction != m_state->m_functions_by_name.end())
    {
        auto functionCall = std::make_unique<CommonExpressionCustomFunctionCall>(std::move(functionCallName));

        if (result.PeekAndRemoveIfTag(TAG_EXPRESSION_FUNCTION_CALL_END) != TAG_EXPRESSION_FUNCTION_CALL_END)
            throw ParsingException(functionCallToken.GetPos(), "Custom functions cannot be called with arguments");

        return std::move(functionCall);
    }

    throw ParsingException(functionCallToken.GetPos(), "Unknown function");
}
