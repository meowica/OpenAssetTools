#include "MenuConverterT6.h"

#include "Menu/AbstractMenuConverter.h"
#include "MenuConversionZoneStateT6.h"
#include "Parsing/Menu/Domain/EventHandler/CommonEventHandlerCondition.h"
#include "Parsing/Menu/Domain/EventHandler/CommonEventHandlerScript.h"
#include "Parsing/Menu/Domain/EventHandler/CommonEventHandlerSetLocalVar.h"
#include "Parsing/Menu/Domain/Expression/CommonExpressionBaseFunctionCall.h"
#include "Parsing/Menu/Domain/Expression/CommonExpressionCustomFunctionCall.h"
#include "Parsing/Menu/MenuAssetZoneState.h"
#include "Parsing/Simple/Expression/SimpleExpressionBinaryOperation.h"
#include "Parsing/Simple/Expression/SimpleExpressionConditionalOperator.h"
#include "Parsing/Simple/Expression/SimpleExpressionUnaryOperation.h"
#include "Utils/ClassUtils.h"
#include "Utils/StringUtils.h"

#include <cassert>
#include <cstring>
#include <format>
#include <sstream>

using namespace T6;
using namespace menu;

namespace
{
    class MenuConverter : public AbstractMenuConverter, public IMenuConverter
    {
        [[nodiscard]] static rectDef_s ConvertRectDef(const CommonRect& rect)
        {
            return rectDef_s{
                static_cast<float>(rect.x),
                static_cast<float>(rect.y),
                static_cast<float>(rect.w),
                static_cast<float>(rect.h),
                static_cast<unsigned char>(rect.horizontalAlign),
                static_cast<unsigned char>(rect.verticalAlign),
            };
        }

        [[nodiscard]] static rectDef_s ConvertRectDefRelativeTo(const CommonRect& rect, const CommonRect& rectRelativeTo)
        {
            return rectDef_s{
                static_cast<float>(rectRelativeTo.x + rect.x),
                static_cast<float>(rectRelativeTo.y + rect.y),
                static_cast<float>(rect.w),
                static_cast<float>(rect.h),
                static_cast<unsigned char>(rect.horizontalAlign),
                static_cast<unsigned char>(rect.verticalAlign),
            };
        }

        static void ConvertColor(float (&output)[4], const CommonColor& input)
        {
            output[0] = static_cast<float>(input.r);
            output[1] = static_cast<float>(input.g);
            output[2] = static_cast<float>(input.b);
            output[3] = static_cast<float>(input.a);
        }

        static void ApplyFlag(int& flags, const bool shouldApply, const int flagValue)
        {
            if (!shouldApply)
                return;

            flags |= flagValue;
        }

        static int ConvertItemType(const int input)
        {
            return input;
        }

        static int ConvertTextFont(const int input)
        {
            return input;
        }

        [[nodiscard]] Material* ConvertMaterial(const std::string& materialName, const CommonMenuDef* menu, const CommonItemDef* item = nullptr) const
        {
            if (materialName.empty())
                return nullptr;

            auto* materialDependency = m_context.LoadDependency<AssetMaterial>(materialName);
            if (!materialDependency)
                throw MenuConversionException(std::format("Failed to load material \"{}\"", materialName), menu, item);

            return materialDependency->Asset();
        }

        [[nodiscard]] ItemKeyHandler* ConvertKeyHandler(const std::multimap<int, std::unique_ptr<CommonEventHandlerSet>>& keyHandlers,
                                                        const CommonMenuDef* menu,
                                                        const CommonItemDef* item = nullptr) const
        {
            if (keyHandlers.empty())
                return nullptr;

            const auto keyHandlerCount = keyHandlers.size();
            auto* output = m_memory.Alloc<ItemKeyHandler>(keyHandlerCount);
            auto currentKeyHandler = keyHandlers.cbegin();
            for (auto i = 0u; i < keyHandlerCount; i++)
            {
                output[i].key = currentKeyHandler->first;
                //output[i].action = ConvertEventHandlerSet(currentKeyHandler->second.get(), menu, item);

                if (i + 1 < keyHandlerCount)
                    output[i].next = &output[i + 1];
                else
                    output[i].next = nullptr;
                ++currentKeyHandler;
            }

            return output;
        }

        [[nodiscard]] const char* CreateEnableDvarString(const std::vector<std::string>& stringElements) const
        {
            std::ostringstream ss;

            for (const auto& element : stringElements)
            {
                ss << "\"" << element << "\" ";
            }

            return m_memory.Dup(ss.str().c_str());
        }

        [[nodiscard]] const char* ConvertEnableDvar(const CommonItemDef& commonItem, int& dvarFlags) const
        {
            dvarFlags = 0;

            if (!commonItem.m_enable_dvar.empty())
            {
                dvarFlags |= ITEM_DVAR_FLAG_ENABLE;
                return CreateEnableDvarString(commonItem.m_enable_dvar);
            }

            if (!commonItem.m_disable_dvar.empty())
            {
                dvarFlags |= ITEM_DVAR_FLAG_DISABLE;
                return CreateEnableDvarString(commonItem.m_disable_dvar);
            }

            if (!commonItem.m_show_dvar.empty())
            {
                dvarFlags |= ITEM_DVAR_FLAG_SHOW;
                return CreateEnableDvarString(commonItem.m_show_dvar);
            }

            if (!commonItem.m_hide_dvar.empty())
            {
                dvarFlags |= ITEM_DVAR_FLAG_HIDE;
                return CreateEnableDvarString(commonItem.m_hide_dvar);
            }

            if (!commonItem.m_focus_dvar.empty())
            {
                dvarFlags |= ITEM_DVAR_FLAG_FOCUS;
                return CreateEnableDvarString(commonItem.m_focus_dvar);
            }

            return nullptr;
        }

        [[nodiscard]] listBoxDef_s* ConvertListBoxFeatures(itemDef_s* item,
                                                           CommonItemFeaturesListBox* commonListBox,
                                                           const CommonMenuDef& parentMenu,
                                                           const CommonItemDef& commonItem) const
        {
            if (commonListBox == nullptr)
                return nullptr;

            auto* listBox = m_memory.Alloc<listBoxDef_s>();
            listBox->notselectable = commonListBox->m_not_selectable ? 1 : 0;
            listBox->noScrollBars = commonListBox->m_no_scrollbars ? 1 : 0;
            listBox->usePaging = commonListBox->m_use_paging ? 1 : 0;
            listBox->elementWidth = static_cast<float>(commonListBox->m_element_width);
            listBox->elementHeight = static_cast<float>(commonListBox->m_element_height);
            //item->special = static_cast<float>(commonListBox->m_feeder);
            //listBox->elementStyle = commonListBox->m_element_style;
            //listBox->onDoubleClick = ConvertEventHandlerSet(commonListBox->m_on_double_click.get(), &parentMenu, &commonItem);
            //ConvertColor(listBox->selectBorder, commonListBox->m_select_border);
            listBox->selectIcon = ConvertMaterial(commonListBox->m_select_icon, &parentMenu, &commonItem);

            listBox->numColumns = static_cast<int>(std::min(std::extent_v<decltype(listBoxDef_s::columnInfo)>, commonListBox->m_columns.size()));
            for (auto i = 0; i < listBox->numColumns; i++)
            {
                auto& col = listBox->columnInfo[i];
                const auto& commonCol = commonListBox->m_columns[i];

                //col.pos = commonCol.m_x_pos;
                //col.width = commonCol.m_width;
                col.maxChars = commonCol.m_max_chars;
                //col.alignment = commonCol.m_alignment;
            }

            return listBox;
        }

        [[nodiscard]] editFieldDef_s* ConvertEditFieldFeatures(itemDef_s* item,
                                                               CommonItemFeaturesEditField* commonEditField,
                                                               const CommonMenuDef& parentMenu,
                                                               const CommonItemDef& commonItem) const
        {
            if (commonEditField == nullptr)
                return nullptr;

            auto* editField = m_memory.Alloc<editFieldDef_s>();
            editField->defVal = static_cast<float>(commonEditField->m_def_val);
            editField->minVal = static_cast<float>(commonEditField->m_min_val);
            editField->maxVal = static_cast<float>(commonEditField->m_max_val);
            //item->localVar = ConvertString(commonEditField->m_local_var);
            editField->maxChars = commonEditField->m_max_chars;
            editField->maxCharsGotoNext = commonEditField->m_max_chars_goto_next ? 1 : 0;
            editField->maxPaintChars = commonEditField->m_max_paint_chars;

            return editField;
        }

        [[nodiscard]] multiDef_s* ConvertMultiValueFeatures(itemDef_s* item,
                                                            CommonItemFeaturesMultiValue* commonMultiValue,
                                                            const CommonMenuDef& parentMenu,
                                                            const CommonItemDef& commonItem) const
        {
            if (commonMultiValue == nullptr)
                return nullptr;

            auto* multiValue = m_memory.Alloc<multiDef_s>();
            multiValue->count = static_cast<int>(std::min(std::extent_v<decltype(multiDef_s::dvarList)>, commonMultiValue->m_step_names.size()));
            multiValue->strDef = !commonMultiValue->m_string_values.empty() ? 1 : 0;

            for (auto i = 0; i < multiValue->count; i++)
            {
                multiValue->dvarList[i] = ConvertString(commonMultiValue->m_step_names[i]);

                if (multiValue->strDef)
                {
                    if (commonMultiValue->m_string_values.size() > static_cast<unsigned>(i))
                        multiValue->dvarStr[i] = ConvertString(commonMultiValue->m_string_values[i]);
                }
                else
                {
                    if (commonMultiValue->m_double_values.size() > static_cast<unsigned>(i))
                        multiValue->dvarValue[i] = static_cast<float>(commonMultiValue->m_double_values[i]);
                }
            }

            return multiValue;
        }

        [[nodiscard]] itemDef_s* ConvertItem(const CommonMenuDef& parentMenu, const CommonItemDef& commonItem) const
        {
            auto* item = m_memory.Alloc<itemDef_s>();

            item->window.name = ConvertString(commonItem.m_name);
            //item->text = ConvertString(commonItem.m_text);
            //ApplyFlag(item->itemFlags, commonItem.m_text_save_game, ITEM_FLAG_SAVE_GAME_INFO);
            //ApplyFlag(item->itemFlags, commonItem.m_text_cinematic_subtitle, ITEM_FLAG_CINEMATIC_SUBTITLE);
            item->window.group = ConvertString(commonItem.m_group);
            item->window.rectClient = ConvertRectDef(commonItem.m_rect);
            item->window.rect = ConvertRectDefRelativeTo(commonItem.m_rect, parentMenu.m_rect);
            item->window.style = commonItem.m_style;
            ApplyFlag(item->window.staticFlags, commonItem.m_decoration, WINDOW_FLAG_DECORATION);
            ApplyFlag(item->window.staticFlags, commonItem.m_auto_wrapped, WINDOW_FLAG_AUTO_WRAPPED);
            ApplyFlag(item->window.staticFlags, commonItem.m_horizontal_scroll, WINDOW_FLAG_HORIZONTAL_SCROLL);
            item->type = ConvertItemType(commonItem.m_type);
            item->dataType = item->type;
            item->window.border = commonItem.m_border;
            item->window.borderSize = static_cast<float>(commonItem.m_border_size);
            //item->visibleExp = ConvertVisibleExpression(&item->window, commonItem.m_visible_expression.get(), &parentMenu, &commonItem);
            //item->disabledExp = ConvertExpression(commonItem.m_disabled_expression.get(), &parentMenu, &commonItem);
            item->window.ownerDraw = commonItem.m_owner_draw;
            item->window.ownerDrawFlags = commonItem.m_owner_draw_flags;
            //item->alignment = commonItem.m_align;
            //item->textAlignMode = commonItem.m_text_align;
            //item->textalignx = static_cast<float>(commonItem.m_text_align_x);
            //item->textaligny = static_cast<float>(commonItem.m_text_align_y);
            //item->textscale = static_cast<float>(commonItem.m_text_scale);
            //item->textStyle = commonItem.m_text_style;
            //item->fontEnum = ConvertTextFont(commonItem.m_text_font);
            //ConvertColor(item->window.backColor, commonItem.m_back_color);

            //ConvertColor(item->window.foreColor, commonItem.m_fore_color);
            if (!commonItem.m_fore_color.Equals(CommonColor(1.0, 1.0, 1.0, 1.0)))
                item->window.dynamicFlags[0] |= WINDOW_FLAG_NON_DEFAULT_FORECOLOR;

            //ConvertColor(item->window.borderColor, commonItem.m_border_color);
            //ConvertColor(item->window.outlineColor, commonItem.m_outline_color);
            //ConvertColor(item->window.disableColor, commonItem.m_disable_color);
            //ConvertColor(item->glowColor, commonItem.m_glow_color);
            item->window.background = ConvertMaterial(commonItem.m_background, &parentMenu, &commonItem);
            //item->onFocus = ConvertEventHandlerSet(commonItem.m_on_focus.get(), &parentMenu, &commonItem);
            //item->leaveFocus = ConvertEventHandlerSet(commonItem.m_on_leave_focus.get(), &parentMenu, &commonItem);
            //item->mouseEnter = ConvertEventHandlerSet(commonItem.m_on_mouse_enter.get(), &parentMenu, &commonItem);
            //item->mouseExit = ConvertEventHandlerSet(commonItem.m_on_mouse_exit.get(), &parentMenu, &commonItem);
            //item->mouseEnterText = ConvertEventHandlerSet(commonItem.m_on_mouse_enter_text.get(), &parentMenu, &commonItem);
            //item->mouseExitText = ConvertEventHandlerSet(commonItem.m_on_mouse_exit_text.get(), &parentMenu, &commonItem);
            //item->action = ConvertEventHandlerSet(commonItem.m_on_action.get(), &parentMenu, &commonItem);
            //item->accept = ConvertEventHandlerSet(commonItem.m_on_accept.get(), &parentMenu, &commonItem);
            //item->focusSound = ConvertSound(commonItem.m_focus_sound, &parentMenu, &commonItem);
            item->dvarTest = ConvertString(commonItem.m_dvar_test);
            item->enableDvar = ConvertEnableDvar(commonItem, item->dvarFlags);
            //item->onKey = ConvertKeyHandler(commonItem.m_key_handlers, &parentMenu, &commonItem);
            //item->textExp = ConvertOrApplyStatement(item->text, commonItem.m_text_expression.get(), &parentMenu, &commonItem);
            //item->materialExp = ConvertOrApplyStatement(item->window.background, commonItem.m_material_expression.get(), &parentMenu, &commonItem);
            //item->disabledExp = ConvertExpression(commonItem.m_disabled_expression.get(), &parentMenu, &commonItem);
            //item->floatExpressions = ConvertFloatExpressions(&commonItem, item, &parentMenu, item->floatExpressionCount);
            //item->gameMsgWindowIndex = commonItem.m_game_message_window_index;
            //item->gameMsgWindowMode = commonItem.m_game_message_window_mode;
            //item->fxLetterTime = commonItem.m_fx_letter_time;
            //item->fxDecayStartTime = commonItem.m_fx_decay_start_time;
            //item->fxDecayDuration = commonItem.m_fx_decay_duration;
            item->dvar = ConvertString(commonItem.m_dvar);

            switch (commonItem.m_feature_type)
            {
            case CommonItemFeatureType::LISTBOX:
                //item->typeData.listBox = ConvertListBoxFeatures(item, commonItem.m_list_box_features.get(), parentMenu, commonItem);
                break;

            case CommonItemFeatureType::EDIT_FIELD:
                //item->typeData.editField = ConvertEditFieldFeatures(item, commonItem.m_edit_field_features.get(), parentMenu, commonItem);
                break;

            case CommonItemFeatureType::MULTI_VALUE:
                //item->typeData.multi = ConvertMultiValueFeatures(item, commonItem.m_multi_value_features.get(), parentMenu, commonItem);
                break;

            case CommonItemFeatureType::ENUM_DVAR:
                //item->typeData.enumDvarName = ConvertString(commonItem.m_enum_dvar_name);
                break;

            case CommonItemFeatureType::NEWS_TICKER:
                //item->typeData.ticker = ConvertNewsTickerFeatures(item, commonItem.m_news_ticker_features.get(), parentMenu, commonItem);
                break;

            case CommonItemFeatureType::NONE:
            default:
                //if (item->type == ITEM_TYPE_TEXT_SCROLL)
                //    item->typeData.scroll = m_memory.Alloc<textScrollDef_s>();

                break;
            }

            return item;
        }

        itemDef_s** ConvertMenuItems(const CommonMenuDef& commonMenu, int& itemCount) const
        {
            if (commonMenu.m_items.empty())
            {
                itemCount = 0;
                return nullptr;
            }

            auto* items = m_memory.Alloc<itemDef_s*>(commonMenu.m_items.size());
            for (auto i = 0u; i < commonMenu.m_items.size(); i++)
                items[i] = ConvertItem(commonMenu, *commonMenu.m_items[i]);

            itemCount = static_cast<int>(commonMenu.m_items.size());

            return items;
        }

    public:
        MenuConverter(const bool disableOptimizations, ISearchPath& searchPath, MemoryManager& memory, AssetCreationContext& context)
            : AbstractMenuConverter(disableOptimizations, searchPath, memory, context),
              m_conversion_zone_state(context.GetZoneAssetCreationState<MenuConversionZoneState>()),
              m_parsing_zone_state(context.GetZoneAssetCreationState<MenuAssetZoneState>())
        {
        }

        void ConvertMenu(const menu::CommonMenuDef& commonMenu, menuDef_t& menu, AssetRegistration<AssetMenu>& registration) override
        {
            try
            {
                menu.window.name = m_memory.Dup(commonMenu.m_name.c_str());
                menu.fullScreen = commonMenu.m_full_screen;
                ApplyFlag(menu.window.staticFlags, commonMenu.m_screen_space, WINDOW_FLAG_SCREEN_SPACE);
                ApplyFlag(menu.window.staticFlags, commonMenu.m_decoration, WINDOW_FLAG_DECORATION);
                menu.window.rect = ConvertRectDef(commonMenu.m_rect);
                menu.window.style = commonMenu.m_style;
                menu.window.border = commonMenu.m_border;
                menu.window.borderSize = static_cast<float>(commonMenu.m_border_size);
                //ConvertColor(menu.window.backColor, commonMenu.m_back_color);
                //ConvertColor(menu.window.foreColor, commonMenu.m_fore_color);
                //ConvertColor(menu.window.borderColor, commonMenu.m_border_color);
                //ConvertColor(menu.focusColor, commonMenu.m_focus_color);
                menu.window.background = ConvertMaterial(commonMenu.m_background, &commonMenu);
                menu.window.ownerDraw = commonMenu.m_owner_draw;
                menu.window.ownerDrawFlags = commonMenu.m_owner_draw_flags;
                ApplyFlag(menu.window.staticFlags, commonMenu.m_out_of_bounds_click, WINDOW_FLAG_OUT_OF_BOUNDS_CLICK);
                menu.soundName = ConvertString(commonMenu.m_sound_loop);
                ApplyFlag(menu.window.staticFlags, commonMenu.m_popup, WINDOW_FLAG_POPUP);
                menu.fadeClamp = static_cast<float>(commonMenu.m_fade_clamp);
                menu.fadeCycle = commonMenu.m_fade_cycle;
                menu.fadeAmount = static_cast<float>(commonMenu.m_fade_amount);
                menu.fadeInAmount = static_cast<float>(commonMenu.m_fade_in_amount);
                menu.blurRadius = static_cast<float>(commonMenu.m_blur_radius);
                ApplyFlag(menu.window.staticFlags, commonMenu.m_legacy_split_screen_scale, WINDOW_FLAG_LEGACY_SPLIT_SCREEN_SCALE);
                ApplyFlag(menu.window.staticFlags, commonMenu.m_hidden_during_scope, WINDOW_FLAG_HIDDEN_DURING_SCOPE);
                ApplyFlag(menu.window.staticFlags, commonMenu.m_hidden_during_flashbang, WINDOW_FLAG_HIDDEN_DURING_FLASH_BANG);
                ApplyFlag(menu.window.staticFlags, commonMenu.m_hidden_during_ui, WINDOW_FLAG_HIDDEN_DURING_UI);
                menu.allowedBinding = ConvertString(commonMenu.m_allowed_binding);
                ApplyFlag(menu.window.staticFlags, commonMenu.m_text_only_focus, WINDOW_FLAG_TEXT_ONLY_FOCUS);
                //menu.visibleExp = ConvertVisibleExpression(&menu.window, commonMenu.m_visible_expression.get(), &commonMenu);
                //menu.rectXExp = ConvertOrApplyStatement(menu.window.rect.x, commonMenu.m_rect_x_exp.get(), &commonMenu);
                //menu.rectYExp = ConvertOrApplyStatement(menu.window.rect.y, commonMenu.m_rect_y_exp.get(), &commonMenu);
                //menu.rectWExp = ConvertOrApplyStatement(menu.window.rect.w, commonMenu.m_rect_w_exp.get(), &commonMenu);
                //menu.rectHExp = ConvertOrApplyStatement(menu.window.rect.h, commonMenu.m_rect_h_exp.get(), &commonMenu);
                //menu.openSoundExp = ConvertExpression(commonMenu.m_open_sound_exp.get(), &commonMenu);
                //menu.closeSoundExp = ConvertExpression(commonMenu.m_close_sound_exp.get(), &commonMenu);
                //menu.onOpen = ConvertEventHandlerSet(commonMenu.m_on_open.get(), &commonMenu);
                //menu.onClose = ConvertEventHandlerSet(commonMenu.m_on_close.get(), &commonMenu);
                //menu.onCloseRequest = ConvertEventHandlerSet(commonMenu.m_on_request_close.get(), &commonMenu);
                //menu.onESC = ConvertEventHandlerSet(commonMenu.m_on_esc.get(), &commonMenu);
                menu.onKey = ConvertKeyHandler(commonMenu.m_key_handlers, &commonMenu);
                menu.items = ConvertMenuItems(commonMenu, menu.itemCount);
                //menu.expressionData = m_conversion_zone_state.m_supporting_data;
            }
            catch (const MenuConversionException& e)
            {
                PrintConversionExceptionDetails(e);
            }
        }

        MenuConversionZoneState& m_conversion_zone_state;
        MenuAssetZoneState& m_parsing_zone_state;
    };
} // namespace

std::unique_ptr<IMenuConverter> IMenuConverter::Create(bool disableOptimizations, ISearchPath& searchPath, MemoryManager& memory, AssetCreationContext& context)
{
    return std::make_unique<MenuConverter>(disableOptimizations, searchPath, memory, context);
}
