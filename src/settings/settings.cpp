#include <string_view>

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "settings/settings.hpp"

using namespace godot;

namespace gdWui
{
    constexpr std::string_view setting_prefix{ "wui/" };

    void add_custom_project_setting(const String name, const Variant& default_value,
                                    const PropertyHint hint = PropertyHint::PROPERTY_HINT_NONE,
                                    const String hint_string = "")
    {
        const godot::String fullString{ setting_prefix.data() + name };

        auto ProjectSettings = ProjectSettings::get_singleton();
        if (ProjectSettings == NULL)
        {
            UtilityFunctions::print("EditorSettings is null\n");
            return;
        }

        if (!ProjectSettings->has_setting(fullString))
            ProjectSettings->set_setting(fullString, default_value);

        ProjectSettings->set_initial_value(fullString, default_value);

        auto setting_info = Dictionary();

        setting_info[Variant("name")] = fullString;
        setting_info[Variant("type")] = default_value.get_type();
        setting_info[Variant("description")] = "descr";
        setting_info[Variant("tip")] = "tooltip";

        if (hint_string.length() > 0)
        {
            setting_info[Variant("hint")] = hint;
            setting_info[Variant("hint_string")] = hint_string;
        }

        ProjectSettings->add_property_info(setting_info);
        ProjectSettings->set_as_basic(fullString, true);
    }

    // to get if you are in an editor: is_editor_hint

    void initLibrary()
    {
        UtilityFunctions::print("init settings\n");

        const Variant testValueHint = 100;
        add_custom_project_setting("DevToolsMode", testValueHint, PropertyHint::PROPERTY_HINT_RANGE,
                                   "0,100,10");
    }
};
