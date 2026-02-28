#include "MenuWriterT6.h"

#include "Game/T6/MenuConstantsT6.h"
#include "Menu/AbstractMenuWriter.h"
#include "ObjWriting.h"

#include <cassert>
#include <cmath>
#include <sstream>

using namespace T6;

// Uncomment this macro to skip interpretative expression dumping
// #define DUMP_NAIVE

#ifdef DUMP_NAIVE
#define DUMP_FUNC WriteStatementNaive
#else
#define DUMP_FUNC WriteStatementSkipInitialUnnecessaryParenthesis
#endif

namespace
{
    class MenuWriter final : public menu::AbstractBaseWriter, public menu::IWriterT6
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
        static constexpr inline vec4_t COLOR_0000{0.0f, 0.0f, 0.0f, 0.0f};
        static constexpr inline vec4_t COLOR_1111{1.0f, 1.0f, 1.0f, 1.0f};

        void WriteColorProperty(const std::string& propertyKey, const vec4_t& propertyValue, const vec4_t& defaultValue) const
        {
            if (std::fabs(propertyValue.r - defaultValue.r) < std::numeric_limits<float>::epsilon()
                && std::fabs(propertyValue.g - defaultValue.g) < std::numeric_limits<float>::epsilon()
                && std::fabs(propertyValue.b - defaultValue.b) < std::numeric_limits<float>::epsilon()
                && std::fabs(propertyValue.a - defaultValue.a) < std::numeric_limits<float>::epsilon())
            {
                return;
            }

            Indent();
            WriteKey(propertyKey);
            m_stream << propertyValue.r << " " << propertyValue.g << " " << propertyValue.b << " " << propertyValue.a << "\n";
        }

        void WriteMenuEventHandlerSet(const char* eventHandlerSet)
        {
            Indent();
            m_stream << "{\n";
            IncIndent();

            DecIndent();
            Indent();
            m_stream << "}\n";
        }

        void WriteMenuEventHandlerSetProperty(const std::string& propertyKey, const char* eventHandlerSetValue)
        {
            if (eventHandlerSetValue == nullptr)
                return;

            Indent();
            m_stream << propertyKey << "\n";
            WriteMenuEventHandlerSet(eventHandlerSetValue);
        }

        void WriteRectProperty(const std::string& propertyKey, const rectDef_s& rect) const
        {
            Indent();
            WriteKey(propertyKey);
            m_stream << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << " " << static_cast<int>(rect.horzAlign) << " "
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

        void WriteItemKeyHandlerProperty(const ItemKeyHandler* itemKeyHandlerValue)
        {
            for (const auto* currentHandler = itemKeyHandlerValue; currentHandler; currentHandler = currentHandler->next)
            {
                if (currentHandler->key >= '!' && currentHandler->key <= '~' && currentHandler->key != '"')
                {
                    std::ostringstream ss;
                    ss << "execKey \"" << static_cast<char>(currentHandler->key) << "\"";
                    WriteMenuEventHandlerSetProperty(ss.str(), currentHandler->keyScript->action);
                }
                else
                {
                    std::ostringstream ss;
                    ss << "execKeyInt " << currentHandler->key;
                    WriteMenuEventHandlerSetProperty(ss.str(), currentHandler->keyScript->action);
                }
            }
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

        void WriteColumnProperty(const std::string& propertyKey, const listBoxDef_s* listBox) const
        {
            if (listBox->numColumns <= 0)
                return;

            Indent();
            WriteKey(propertyKey);
            m_stream << listBox->numColumns << "\n";

            for (auto col = 0; col < listBox->numColumns; col++)
            {
                Indent();
                for (auto i = 0u; i < MENU_KEY_SPACING; i++)
                    m_stream << " ";

                m_stream << listBox->columnInfo[col].rect.x << " "
                         << listBox->columnInfo[col].rect.y << " "
                         << listBox->columnInfo[col].rect.w << " "
                         << listBox->columnInfo[col].rect.h << " "
                         << listBox->columnInfo[col].maxChars
                         << " "
                         << listBox->columnInfo[col].rect.horzAlign << " "
                         << listBox->columnInfo[col].rect.vertAlign << "\n";
            }
        }

        void WriteListBoxProperties(const itemDef_s* item)
        {
            if (item->type != ITEM_TYPE_LISTBOX || item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.listBox == nullptr)
                return;

            const auto* listBox = item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.listBox;
            WriteKeywordProperty("notselectable", listBox->notselectable != 0);
            WriteKeywordProperty("noscrollbars", listBox->noScrollBars != 0);
            WriteKeywordProperty("usepaging", listBox->usePaging != 0);
            WriteFloatProperty("elementwidth", listBox->elementWidth, 0.0f);
            WriteFloatProperty("elementheight", listBox->elementHeight, 0.0f);
            WriteFloatProperty("feeder", listBox->special, 0.0f);
            WriteIntProperty("elementtype", listBox->columnInfo->elementStyle, 0);
            WriteColumnProperty("columns", listBox);
            WriteColorProperty("selectBorder", listBox->selectBorder, COLOR_0000);
            WriteMaterialProperty("selectIcon", listBox->selectIcon);
        }

        void WriteDvarFloatProperty(const std::string& propertyKey, const itemDef_s* item, const editFieldDef_s* editField) const
        {
            if (item->dvar == nullptr)
                return;

            Indent();
            WriteKey(propertyKey);
            m_stream << "\"" << item->dvar << "\" " << " " << editField->minVal << " " << editField->maxVal << "\n";
        }

        void WriteEditFieldProperties(const itemDef_s* item) const
        {
            switch (item->type)
            {
            case ITEM_TYPE_TEXT:
            case ITEM_TYPE_EDITFIELD:
            case ITEM_TYPE_NUMERICFIELD:
            case ITEM_TYPE_SLIDER:
            case ITEM_TYPE_YESNO:
            case ITEM_TYPE_BIND:
            case ITEM_TYPE_VALIDFILEFIELD:
            case ITEM_TYPE_UPREDITFIELD:
                break;

            default:
                return;
            }

            if (item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.editField == nullptr)
                return;

            const auto* editField = item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.editField;
            if (std::fabs(-1.0f - editField->minVal) >= std::numeric_limits<float>::epsilon()
                || std::fabs(-1.0f - editField->maxVal) >= std::numeric_limits<float>::epsilon())
            {
                WriteDvarFloatProperty("dvarFloat", item, editField);
            }
            else
            {
                WriteStringProperty("dvar", item->dvar);
            }
            WriteIntProperty("maxChars", editField->maxChars, 0);
            WriteKeywordProperty("maxCharsGotoNext", editField->maxCharsGotoNext != 0);
            WriteIntProperty("maxPaintChars", editField->maxPaintChars, 0);
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
            if (item->type != ITEM_TYPE_MULTI || item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.multi == nullptr)
                return;

            const auto* multiDef = item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.multi;

            if (multiDef->count <= 0)
                return;

            WriteStringProperty("dvar", item->dvar);
            WriteMultiValueProperty(multiDef);
        }

        void WriteEnumDvarProperties(const itemDef_s* item) const
        {
            if (item->type != ITEM_TYPE_DVARENUM)
                return;

            WriteStringProperty("dvar", item->dvar);
            WriteStringProperty("dvarEnumList", item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.enumDvar->enumDvarName);
        }

        void WriteItemData(const itemDef_s* item)
        {
            WriteStringProperty("name", item->window.name);
            //WriteStringProperty("text", item->typeData.textDef->text);
            //WriteKeywordProperty("textsavegame", item->typeData.textDef->itemFlags & ITEM_FLAG_SAVE_GAME_INFO);
            //WriteKeywordProperty("textcinematicsubtitle", item->typeData.textDef->itemFlags & ITEM_FLAG_CINEMATIC_SUBTITLE);
            WriteStringProperty("group", item->window.group);
            WriteRectProperty("rect", item->window.rectClient);
            WriteIntProperty("style", item->window.style, 0);
            //WriteKeywordProperty("decoration", item->window.staticFlags & WINDOW_FLAG_DECORATION);
            //WriteKeywordProperty("autowrapped", item->window.staticFlags & WINDOW_FLAG_AUTO_WRAPPED);
            //WriteKeywordProperty("horizontalscroll", item->window.staticFlags & WINDOW_FLAG_HORIZONTAL_SCROLL);
            WriteIntProperty("type", item->type, ITEM_TYPE_TEXT);
            WriteIntProperty("border", item->window.border, 0);
            WriteFloatProperty("borderSize", item->window.borderSize, 0.0f);

            WriteIntProperty("ownerdraw", item->window.ownerDraw, 0);
            WriteFlagsProperty("ownerdrawFlag", item->window.ownerDrawFlags);
            //WriteIntProperty("align", item->typeData.textDef->alignment, 0);
            //WriteIntProperty("textalign", item->typeData.textDef->textAlignMode, 0);
            //WriteFloatProperty("textalignx", item->typeData.textDef->textalignx, 0.0f);
            //WriteFloatProperty("textaligny", item->typeData.textDef->textaligny, 0.0f);
            //WriteFloatProperty("textscale", item->typeData.textDef->textscale, 0.0f);
            //WriteIntProperty("textstyle", item->typeData.textDef->textStyle, 0);
            //WriteIntProperty("textfont", item->typeData.textDef->fontEnum, 0);
            //WriteColorProperty("backcolor", item->window.backColor, COLOR_0000);
            //WriteColorProperty("forecolor", item->window.foreColor, COLOR_1111);
            //WriteColorProperty("bordercolor", item->window.borderColor, COLOR_0000);
            //WriteColorProperty("outlinecolor", item->window.outlineColor, COLOR_0000);
            //WriteColorProperty("disablecolor", item->typeData.textDef->textTypeData.focusItemDef->focusTypeData.listBox->disableColor, COLOR_0000);
            //WriteColorProperty("glowcolor", item->typeData.textDef->glowColor, COLOR_0000);
            WriteMaterialProperty("background", item->window.background);
            //WriteMenuEventHandlerSetProperty("mouseEnter", item->typeData.textDef->textTypeData.focusItemDef->mouseEnter);
            //WriteMenuEventHandlerSetProperty("mouseExit", item->typeData.textDef->textTypeData.focusItemDef->mouseExit);
            //WriteMenuEventHandlerSetProperty("mouseEnterText", item->typeData.textDef->textTypeData.focusItemDef->mouseEnterText);
            //WriteMenuEventHandlerSetProperty("mouseExitText", item->typeData.textDef->textTypeData.focusItemDef->mouseExitText);
            WriteStringProperty("dvarTest", item->dvarTest);

            //if (item->dvarFlags & ITEM_DVAR_FLAG_ENABLE)
            //    WriteMultiTokenStringProperty("enableDvar", item->enableDvar);
            //else if (item->dvarFlags & ITEM_DVAR_FLAG_DISABLE)
            //    WriteMultiTokenStringProperty("disableDvar", item->enableDvar);
            //else if (item->dvarFlags & ITEM_DVAR_FLAG_SHOW)
            //    WriteMultiTokenStringProperty("showDvar", item->enableDvar);
            //else if (item->dvarFlags & ITEM_DVAR_FLAG_HIDE)
            //    WriteMultiTokenStringProperty("hideDvar", item->enableDvar);
            //else if (item->dvarFlags & ITEM_DVAR_FLAG_FOCUS)
            //    WriteMultiTokenStringProperty("focusDvar", item->enableDvar);

            //WriteItemKeyHandlerProperty(item->typeData.textDef->textTypeData.focusItemDef->onKey);
            //WriteIntProperty("gamemsgwindowindex", item->typeData.textDef->textTypeData.gameMsgDef->gameMsgWindowIndex, 0);
            //WriteIntProperty("gamemsgwindowmode", item->typeData.textDef->textTypeData.gameMsgDef->gameMsgWindowMode, 0);

            WriteListBoxProperties(item);
            //WriteEditFieldProperties(item);
            WriteMultiProperties(item);
            WriteEnumDvarProperties(item);
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
            //WriteKeywordProperty("screenSpace", menu->window.staticFlags & WINDOW_FLAG_SCREEN_SPACE);
            //WriteKeywordProperty("decoration", menu->window.staticFlags & WINDOW_FLAG_DECORATION);
            WriteRectProperty("rect", menu->window.rect);
            WriteIntProperty("style", menu->window.style, 0);
            WriteIntProperty("border", menu->window.border, 0);
            WriteFloatProperty("borderSize", menu->window.borderSize, 0.0f);
            WriteColorProperty("backcolor", menu->window.backColor, COLOR_0000);
            WriteColorProperty("forecolor", menu->window.foreColor, COLOR_1111);
            WriteColorProperty("bordercolor", menu->window.borderColor, COLOR_0000);
            WriteColorProperty("focuscolor", menu->focusColor, COLOR_0000);
            WriteColorProperty("outlinecolor", menu->window.outlineColor, COLOR_0000);
            WriteMaterialProperty("background", menu->window.background);
            WriteIntProperty("ownerdraw", menu->window.ownerDraw, 0);
            WriteFlagsProperty("ownerdrawFlag", menu->window.ownerDrawFlags);
            //WriteKeywordProperty("outOfBoundsClick", menu->window.staticFlags & WINDOW_FLAG_OUT_OF_BOUNDS_CLICK);
            WriteStringProperty("soundLoop", menu->soundName);
            //WriteKeywordProperty("popup", menu->window.staticFlags & WINDOW_FLAG_POPUP);
            WriteFloatProperty("fadeClamp", menu->fadeClamp, 0.0f);
            WriteIntProperty("fadeCycle", menu->fadeCycle, 0);
            WriteFloatProperty("fadeAmount", menu->fadeAmount, 0.0f);
            WriteFloatProperty("fadeInAmount", menu->fadeInAmount, 0.0f);
            WriteFloatProperty("blurWorld", menu->blurRadius, 0.0f);
            //WriteKeywordProperty("legacySplitScreenScale", menu->window.staticFlags & WINDOW_FLAG_LEGACY_SPLIT_SCREEN_SCALE);
            //WriteKeywordProperty("hiddenDuringScope", menu->window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_SCOPE);
            //WriteKeywordProperty("hiddenDuringFlashbang", menu->window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_FLASH_BANG);
            //WriteKeywordProperty("hiddenDuringUI", menu->window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_UI);
            WriteStringProperty("allowedBinding", menu->allowedBinding);
            //WriteKeywordProperty("textOnlyFocus", menu->window.staticFlags & WINDOW_FLAG_TEXT_ONLY_FOCUS);

            WriteItemKeyHandlerProperty(menu->onKey);
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
