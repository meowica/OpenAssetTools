#include "Menu/AbstractMenuWriter.h"
#include "MenuWriterT6.h"
#include "ObjWriting.h"

#include <cassert>
#include <cmath>
#include <sstream>

using namespace T6;

namespace
{
    class MenuWriter final : public ::menu::AbstractBaseWriter, public menu::IWriterT6
    {
    public:
        explicit MenuWriter(std::ostream& stream)
            : AbstractBaseWriter(stream)
        {
        }

        void WriteMenu(const menuDef_t& menu) override
        {
            StartMenuDefScope();
            WriteMenuData(&menu);
            EndScope();
        }

        void Start() override
        {
            AbstractBaseWriter::Start();
        }

        void End() override
        {
            AbstractBaseWriter::End();
        }

        void IncludeMenu(const std::string& menuPath) const override
        {
            AbstractBaseWriter::IncludeMenu(menuPath);
        }

    private:
        // #define WRITE_ORIGINAL_SCRIPT
        void WriteUnconditionalScript(const char* script) const
        {
#ifdef WRITE_ORIGINAL_SCRIPT
            Indent();
            m_stream << script << "\n";
            return;
#endif

            const auto tokenList = CreateScriptTokenList(script);

            auto isNewStatement = true;
            for (const auto& token : tokenList)
            {
                if (isNewStatement)
                {
                    if (token == ";")
                        continue;

                    Indent();
                }

                if (token == ";")
                {
                    m_stream << ";\n";
                    isNewStatement = true;
                    continue;
                }

                if (!isNewStatement)
                    m_stream << " ";
                else
                    isNewStatement = false;

                if (DoesTokenNeedQuotationMarks(token))
                    m_stream << "\"" << token << "\"";
                else
                    m_stream << token;
            }

            if (!isNewStatement)
                m_stream << ";\n";
        }

        //void WriteMenuEventHandlerSet(const MenuEventHandlerSet* eventHandlerSet)
        //{
        //    Indent();
        //    m_stream << "{\n";
        //    IncIndent();

        //    for (auto i = 0; i < eventHandlerSet->eventHandlerCount; i++)
        //    {
        //        const auto* eventHandler = eventHandlerSet->eventHandlers[i];
        //        if (eventHandler == nullptr)
        //            continue;

        //        switch (eventHandler->eventType)
        //        {
        //        case EVENT_UNCONDITIONAL:
        //            WriteUnconditionalScript(eventHandler->eventData.unconditionalScript);
        //            break;

        //        case EVENT_IF:
        //            if (eventHandler->eventData.conditionalScript == nullptr || eventHandler->eventData.conditionalScript->eventExpression == nullptr
        //                || eventHandler->eventData.conditionalScript->eventHandlerSet == nullptr)
        //            {
        //                continue;
        //            }

        //            Indent();
        //            m_stream << "if (";
        //            WriteStatementSkipInitialUnnecessaryParenthesis(eventHandler->eventData.conditionalScript->eventExpression);
        //            m_stream << ")\n";
        //            WriteMenuEventHandlerSet(eventHandler->eventData.conditionalScript->eventHandlerSet);
        //            break;

        //        case EVENT_ELSE:
        //            if (eventHandler->eventData.elseScript == nullptr)
        //                continue;

        //            Indent();
        //            m_stream << "else\n";
        //            WriteMenuEventHandlerSet(eventHandler->eventData.elseScript);
        //            break;

        //        case EVENT_SET_LOCAL_VAR_BOOL:
        //            WriteSetLocalVarData("setLocalVarBool", eventHandler->eventData.setLocalVarData);
        //            break;

        //        case EVENT_SET_LOCAL_VAR_INT:
        //            WriteSetLocalVarData("setLocalVarInt", eventHandler->eventData.setLocalVarData);
        //            break;

        //        case EVENT_SET_LOCAL_VAR_FLOAT:
        //            WriteSetLocalVarData("setLocalVarFloat", eventHandler->eventData.setLocalVarData);
        //            break;

        //        case EVENT_SET_LOCAL_VAR_STRING:
        //            WriteSetLocalVarData("setLocalVarString", eventHandler->eventData.setLocalVarData);
        //            break;

        //        default:
        //            break;
        //        }
        //    }

        //    DecIndent();
        //    Indent();
        //    m_stream << "}\n";
        //}

        //void WriteMenuEventHandlerSetProperty(const std::string& propertyKey, const MenuEventHandlerSet* eventHandlerSetValue)
        //{
        //    if (eventHandlerSetValue == nullptr)
        //        return;

        //    Indent();
        //    m_stream << propertyKey << "\n";
        //    WriteMenuEventHandlerSet(eventHandlerSetValue);
        //}

        void WriteRectProperty(const std::string& propertyKey, const rectDef_s& rect) const
        {
            Indent();
            WriteKey(propertyKey);

            m_stream << rect.x << " "
                     << rect.y << " "
                     << rect.w << " "
                     << rect.h << " "
                     << static_cast<int>(rect.horzAlign) << " "
                     << static_cast<int>(rect.vertAlign) << "\n";
        }

        void WriteMaterialProperty(const std::string& propertyKey, const Material* materialValue) const
        {
            if (materialValue == nullptr || materialValue->info.name == nullptr)
                return;

            if (materialValue->info.name[0] == ',')
                WriteStringProperty(propertyKey, &materialValue->info.name[1]);
            else
                WriteStringProperty(propertyKey, materialValue->info.name);
        }

        void WriteDecodeEffectProperty(const std::string& propertyKey, const itemDef_s* item) const
        {
            //if (!item->decayActive)
            //    return;

            //Indent();
            //WriteKey(propertyKey);
            //m_stream << item->fxLetterTime << " " << item->fxDecayStartTime << " " << item->fxDecayDuration << "\n";
        }

        void WriteItemKeyHandlerProperty(const ItemKeyHandler* itemKeyHandlerValue)
        {
            //for (const auto* currentHandler = itemKeyHandlerValue; currentHandler; currentHandler = currentHandler->next)
            //{
            //    if (currentHandler->key >= '!' && currentHandler->key <= '~' && currentHandler->key != '"')
            //    {
            //        std::ostringstream ss;
            //        ss << "execKey \"" << static_cast<char>(currentHandler->key) << "\"";
            //        WriteMenuEventHandlerSetProperty(ss.str(), currentHandler->action);
            //    }
            //    else
            //    {
            //        std::ostringstream ss;
            //        ss << "execKeyInt " << currentHandler->key;
            //        WriteMenuEventHandlerSetProperty(ss.str(), currentHandler->action);
            //    }
            //}
        }

        void WriteMultiTokenStringProperty(const std::string& propertyKey, const char* value) const
        {
            if (!value)
                return;

            Indent();
            WriteKey(propertyKey);

            const auto tokenList = CreateScriptTokenList(value);

            auto firstToken = true;

            m_stream << "{ ";
            for (const auto& token : tokenList)
            {
                if (firstToken)
                    firstToken = false;
                else
                    m_stream << ";";
                m_stream << "\"" << token << "\"";
            }

            if (!firstToken)
                m_stream << " ";
            m_stream << "}\n";
        }

        //void WriteFloatExpressionsProperty(const ItemFloatExpression* floatExpressions, int floatExpressionCount) const
        //{
        //    if (!floatExpressions)
        //        return;

        //    for (int i = 0; i < floatExpressionCount; i++)
        //    {
        //        const auto& floatExpression = floatExpressions[i];

        //        if (floatExpression.target < 0 || floatExpression.target >= ITEM_FLOATEXP_TGT_COUNT)
        //            continue;

        //        std::string propertyName = std::string("exp ") + floatExpressionTargetBindings[floatExpression.target].name + std::string(" ")
        //                                   + floatExpressionTargetBindings[floatExpression.target].componentName;

        //        WriteStatementProperty(propertyName, floatExpression.expression, false);
        //    }
        //}

        void WriteColumnProperty(const std::string& propertyKey, const listBoxDef_s* listBox) const
        {
            //if (listBox->numColumns <= 0)
            //    return;

            //Indent();
            //WriteKey(propertyKey);
            //m_stream << listBox->numColumns << "\n";

            //for (auto col = 0; col < listBox->numColumns; col++)
            //{
            //    Indent();
            //    for (auto i = 0u; i < MENU_KEY_SPACING; i++)
            //        m_stream << " ";

            //    m_stream << listBox->columnInfo[col].pos << " " << listBox->columnInfo[col].width << " " << listBox->columnInfo[col].maxChars << " "
            //             << listBox->columnInfo[col].alignment << "\n";
            //}
        }

        void WriteListBoxProperties(const itemDef_s* item)
        {
            //if (item->type != ITEM_TYPE_LISTBOX || item->typeData.listBox == nullptr)
            //    return;

            //const auto* listBox = item->typeData.listBox;
            //WriteKeywordProperty("notselectable", listBox->notselectable != 0);
            //WriteKeywordProperty("noscrollbars", listBox->noScrollBars != 0);
            //WriteKeywordProperty("usepaging", listBox->usePaging != 0);
            //WriteFloatProperty("elementwidth", listBox->elementWidth, 0.0f);
            //WriteFloatProperty("elementheight", listBox->elementHeight, 0.0f);
            //WriteFloatProperty("feeder", item->special, 0.0f);
            //WriteIntProperty("elementtype", listBox->elementStyle, 0);
            //WriteColumnProperty("columns", listBox);
            //WriteMenuEventHandlerSetProperty("doubleclick", listBox->onDoubleClick);
            //WriteColorProperty("selectBorder", listBox->selectBorder, COLOR_0000);
            //WriteMaterialProperty("selectIcon", listBox->selectIcon);
        }

        void WriteDvarFloatProperty(const std::string& propertyKey, const itemDef_s* item, const editFieldDef_s* editField) const
        {
            //if (item->dvar == nullptr)
            //    return;

            //Indent();
            //WriteKey(propertyKey);
            //m_stream << "\"" << item->dvar << "\" " << editField->defVal << " " << editField->minVal << " " << editField->maxVal << "\n";
        }

        void WriteEditFieldProperties(const itemDef_s* item) const
        {
            //switch (item->type)
            //{
            //case ITEM_TYPE_TEXT:
            //case ITEM_TYPE_EDITFIELD:
            //case ITEM_TYPE_NUMERICFIELD:
            //case ITEM_TYPE_SLIDER:
            //case ITEM_TYPE_YESNO:
            //case ITEM_TYPE_BIND:
            //case ITEM_TYPE_VALIDFILEFIELD:
            //case ITEM_TYPE_DECIMALFIELD:
            //case ITEM_TYPE_UPREDITFIELD:
            //case ITEM_TYPE_EMAILFIELD:
            //case ITEM_TYPE_PASSWORDFIELD:
            //    break;

            //default:
            //    return;
            //}

            //if (item->typeData.editField == nullptr)
            //    return;

            //const auto* editField = item->typeData.editField;
            //if (std::fabs(-1.0f - editField->defVal) >= std::numeric_limits<float>::epsilon()
            //    || std::fabs(-1.0f - editField->minVal) >= std::numeric_limits<float>::epsilon()
            //    || std::fabs(-1.0f - editField->maxVal) >= std::numeric_limits<float>::epsilon())
            //{
            //    WriteDvarFloatProperty("dvarFloat", item, editField);
            //}
            //else
            //{
            //    WriteStringProperty("dvar", item->dvar);
            //}
            //WriteStringProperty("localvar", item->localVar);
            //WriteIntProperty("maxChars", editField->maxChars, 0);
            //WriteKeywordProperty("maxCharsGotoNext", editField->maxCharsGotoNext != 0);
            //WriteIntProperty("maxPaintChars", editField->maxPaintChars, 0);
        }

        void WriteMultiValueProperty(const multiDef_s* multiDef) const
        {
            Indent();

            if (multiDef->strDef)
                WriteKey("dvarStrList");
            else
                WriteKey("dvarFloatList");

            m_stream << "{";
            for (auto i = 0; i < multiDef->count; i++)
            {
                if (multiDef->dvarList[i] == nullptr || multiDef->strDef && multiDef->dvarStr[i] == nullptr)
                    continue;

                m_stream << " \"" << multiDef->dvarList[i] << "\"";

                if (multiDef->strDef)
                    m_stream << " \"" << multiDef->dvarStr[i] << "\"";
                else
                    m_stream << " " << multiDef->dvarValue[i] << "";
            }
            m_stream << " }\n";
        }

        void WriteMultiProperties(const itemDef_s* item) const
        {
            //if (item->type != ITEM_TYPE_MULTI || item->typeData.multi == nullptr)
            //    return;

            //const auto* multiDef = item->typeData.multi;

            //if (multiDef->count <= 0)
            //    return;

            //WriteStringProperty("dvar", item->dvar);
            //WriteStringProperty("localvar", item->localVar);
            //WriteMultiValueProperty(multiDef);
        }

        void WriteEnumDvarProperties(const itemDef_s* item) const
        {
            //if (item->type != ITEM_TYPE_DVARENUM)
            //    return;

            //WriteStringProperty("dvar", item->dvar);
            //WriteStringProperty("localvar", item->localVar);
            //WriteStringProperty("dvarEnumList", item->typeData.enumDvarName);
        }

        void WriteTickerProperties(const itemDef_s* item) const
        {
            //if (item->type != ITEM_TYPE_NEWS_TICKER || item->typeData.ticker == nullptr)
            //    return;

            //const auto* newsTickerDef = item->typeData.ticker;
            //WriteIntProperty("spacing", newsTickerDef->spacing, 0);
            //WriteIntProperty("speed", newsTickerDef->speed, 0);
            //WriteIntProperty("newsfeed", newsTickerDef->feedId, 0);
        }

        void WriteItemData(const itemDef_s* item)
        {
            WriteStringProperty("name", item->window.name);
            //WriteStringProperty("text", item->text);
            //WriteKeywordProperty("textsavegame", item->itemFlags & ITEM_FLAG_SAVE_GAME_INFO);
            //WriteKeywordProperty("textcinematicsubtitle", item->itemFlags & ITEM_FLAG_CINEMATIC_SUBTITLE);
            WriteStringProperty("group", item->window.group);
            WriteRectProperty("rect", item->window.rectClient);
            WriteIntProperty("style", item->window.style, 0);
            WriteKeywordProperty("decoration", item->window.staticFlags & WINDOW_FLAG_DECORATION);
            WriteKeywordProperty("autowrapped", item->window.staticFlags & WINDOW_FLAG_AUTO_WRAPPED);
            WriteKeywordProperty("horizontalscroll", item->window.staticFlags & WINDOW_FLAG_HORIZONTAL_SCROLL);
            WriteIntProperty("type", item->type, ITEM_TYPE_TEXT);
            WriteIntProperty("border", item->window.border, 0);
            WriteFloatProperty("borderSize", item->window.borderSize, 0.0f);

            //if (item->visibleExp)
            //    WriteStatementProperty("visible", item->visibleExp, true);
            //else if (item->window.dynamicFlags[0] & WINDOW_FLAG_VISIBLE)
            //    WriteIntProperty("visible", 1, 0);

            //WriteStatementProperty("disabled", item->disabledExp, true);
            WriteIntProperty("ownerdraw", item->window.ownerDraw, 0);
            WriteFlagsProperty("ownerdrawFlag", item->window.ownerDrawFlags);
            //WriteIntProperty("align", item->alignment, 0);
            //WriteIntProperty("textalign", item->textAlignMode, 0);
            //WriteFloatProperty("textalignx", item->textalignx, 0.0f);
            //WriteFloatProperty("textaligny", item->textaligny, 0.0f);
            //WriteFloatProperty("textscale", item->textscale, 0.0f);
            //WriteIntProperty("textstyle", item->textStyle, 0);
            //WriteIntProperty("textfont", item->fontEnum, 0);
            //WriteColorProperty("backcolor", item->window.backColor, COLOR_0000);
            //WriteColorProperty("forecolor", item->window.foreColor, COLOR_1111);
            //WriteColorProperty("bordercolor", item->window.borderColor, COLOR_0000);
            //WriteColorProperty("outlinecolor", item->window.outlineColor, COLOR_0000);
            //WriteColorProperty("disablecolor", item->window.disableColor, COLOR_0000);
            //WriteColorProperty("glowcolor", item->glowColor, COLOR_0000);
            WriteMaterialProperty("background", item->window.background);
            //WriteMenuEventHandlerSetProperty("onFocus", item->onFocus);
            //WriteMenuEventHandlerSetProperty("leaveFocus", item->leaveFocus);
            //WriteMenuEventHandlerSetProperty("mouseEnter", item->mouseEnter);
            //WriteMenuEventHandlerSetProperty("mouseExit", item->mouseExit);
            //WriteMenuEventHandlerSetProperty("mouseEnterText", item->mouseEnterText);
            //WriteMenuEventHandlerSetProperty("mouseExitText", item->mouseExitText);
            //WriteMenuEventHandlerSetProperty("action", item->action);
            //WriteMenuEventHandlerSetProperty("accept", item->accept);
            //// WriteFloatProperty("special", item->special, 0.0f);
            //WriteSoundAliasProperty("focusSound", item->focusSound);
            WriteStringProperty("dvarTest", item->dvarTest);

            if (item->dvarFlags & ITEM_DVAR_FLAG_ENABLE)
                WriteMultiTokenStringProperty("enableDvar", item->enableDvar);
            else if (item->dvarFlags & ITEM_DVAR_FLAG_DISABLE)
                WriteMultiTokenStringProperty("disableDvar", item->enableDvar);
            else if (item->dvarFlags & ITEM_DVAR_FLAG_SHOW)
                WriteMultiTokenStringProperty("showDvar", item->enableDvar);
            else if (item->dvarFlags & ITEM_DVAR_FLAG_HIDE)
                WriteMultiTokenStringProperty("hideDvar", item->enableDvar);
            else if (item->dvarFlags & ITEM_DVAR_FLAG_FOCUS)
                WriteMultiTokenStringProperty("focusDvar", item->enableDvar);

            //WriteItemKeyHandlerProperty(item->onKey);
            //WriteStatementProperty("exp text", item->textExp, false);
            //WriteStatementProperty("exp material", item->materialExp, false);
            //WriteFloatExpressionsProperty(item->floatExpressions, item->floatExpressionCount);
            //WriteIntProperty("gamemsgwindowindex", item->gameMsgWindowIndex, 0);
            //WriteIntProperty("gamemsgwindowmode", item->gameMsgWindowMode, 0);
            //WriteDecodeEffectProperty("decodeEffect", item);

            //WriteListBoxProperties(item);
            //WriteEditFieldProperties(item);
            //WriteMultiProperties(item);
            //WriteEnumDvarProperties(item);
            //WriteTickerProperties(item);
        }

        void WriteItemDefs(const itemDef_s* const* itemDefs, size_t itemCount)
        {
            for (auto i = 0u; i < itemCount; i++)
            {
                StartItemDefScope();
                WriteItemData(itemDefs[i]);
                EndScope();
            }
        }

        void WriteMenuData(const menuDef_t* menu)
        {
            WriteStringProperty("name", menu->window.name);
            WriteBoolProperty("fullscreen", menu->fullScreen, false);
            WriteKeywordProperty("screenSpace", menu->window.staticFlags & WINDOW_FLAG_SCREEN_SPACE);
            WriteKeywordProperty("decoration", menu->window.staticFlags & WINDOW_FLAG_DECORATION);
            WriteRectProperty("rect", menu->window.rect);
            WriteIntProperty("style", menu->window.style, 0);
            WriteIntProperty("border", menu->window.border, 0);
            WriteFloatProperty("borderSize", menu->window.borderSize, 0.0f);
            //WriteColorProperty("backcolor", menu->window.backColor, COLOR_0000);
            //WriteColorProperty("forecolor", menu->window.foreColor, COLOR_1111);
            //WriteColorProperty("bordercolor", menu->window.borderColor, COLOR_0000);
            //WriteColorProperty("focuscolor", menu->focusColor, COLOR_0000);
            //WriteColorProperty("outlinecolor", menu->window.outlineColor, COLOR_0000);
            WriteMaterialProperty("background", menu->window.background);
            WriteIntProperty("ownerdraw", menu->window.ownerDraw, 0);
            WriteFlagsProperty("ownerdrawFlag", menu->window.ownerDrawFlags);
            WriteKeywordProperty("outOfBoundsClick", menu->window.staticFlags & WINDOW_FLAG_OUT_OF_BOUNDS_CLICK);
            WriteStringProperty("soundLoop", menu->soundName);
            WriteKeywordProperty("popup", menu->window.staticFlags & WINDOW_FLAG_POPUP);
            WriteFloatProperty("fadeClamp", menu->fadeClamp, 0.0f);
            WriteIntProperty("fadeCycle", menu->fadeCycle, 0);
            WriteFloatProperty("fadeAmount", menu->fadeAmount, 0.0f);
            WriteFloatProperty("fadeInAmount", menu->fadeInAmount, 0.0f);
            WriteFloatProperty("blurWorld", menu->blurRadius, 0.0f);
            WriteKeywordProperty("legacySplitScreenScale", menu->window.staticFlags & WINDOW_FLAG_LEGACY_SPLIT_SCREEN_SCALE);
            WriteKeywordProperty("hiddenDuringScope", menu->window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_SCOPE);
            WriteKeywordProperty("hiddenDuringFlashbang", menu->window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_FLASH_BANG);
            WriteKeywordProperty("hiddenDuringUI", menu->window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_UI);
            WriteStringProperty("allowedBinding", menu->allowedBinding);
            WriteKeywordProperty("textOnlyFocus", menu->window.staticFlags & WINDOW_FLAG_TEXT_ONLY_FOCUS);

            //if (menu->visibleExp)
            //    WriteStatementProperty("visible", menu->visibleExp, true);
            //else if (menu->window.dynamicFlags[0] & WINDOW_FLAG_VISIBLE)
            //    WriteIntProperty("visible", 1, 0);

            //WriteStatementProperty("exp rect X", menu->rectXExp, false);
            //WriteStatementProperty("exp rect Y", menu->rectYExp, false);
            //WriteStatementProperty("exp rect W", menu->rectWExp, false);
            //WriteStatementProperty("exp rect H", menu->rectHExp, false);
            //WriteStatementProperty("exp openSound", menu->openSoundExp, false);
            //WriteStatementProperty("exp closeSound", menu->closeSoundExp, false);
            //WriteMenuEventHandlerSetProperty("onOpen", menu->onOpen);
            //WriteMenuEventHandlerSetProperty("onClose", menu->onClose);
            //WriteMenuEventHandlerSetProperty("onRequestClose", menu->onCloseRequest);
            //WriteMenuEventHandlerSetProperty("onESC", menu->onESC);
            //WriteItemKeyHandlerProperty(menu->onKey);
            WriteItemDefs(menu->items, menu->itemCount);
        }
    };
} // namespace

namespace menu
{
    std::unique_ptr<IWriterT6> CreateMenuWriterT6(std::ostream& stream)
    {
        return std::make_unique<MenuWriter>(stream);
    }
} // namespace menu
