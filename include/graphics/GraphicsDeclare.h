/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <string>

#include "base/def/PlatformDefine.h"
#include "base/data/Serializable.h"


namespace sf {

    enum KeyDownFlag {
        Control = 0x01,
        Shift = 0x02,
        LButton = 0x04,
        MButton = 0x08,
        RButton = 0x10,
    };


    enum Visibility {
        Visible,
        Invisible,
        Gone,
    };


    enum Gravity {
        Default = 0x00,
        Left = 0x01,
        Top = 0x02,
        Right = 0x04,
        Bottom = 0x08,

        CenterHorizontal = 0x10,
        CenterVertical = 0x20,
        Center = CenterHorizontal | CenterVertical,

        TopLeft = Top | Left,
        TopRight = Top | Right,
        BottomLeft = Bottom | Left,
        BottomRight = Bottom | Right
    };


    enum Orientation {
        Horizontal,
        Vertical
    };


    enum ViewState {
        StateNormal = 0x00,

        MouseHover = 0x01,
        Pressed = 0x02,
        Enabled = 0x04,
        Selected = 0x08,

        UnMouseHovered = 0x10,
        UnPressed = 0x20,
        ViewDisabled = 0x40,
        UnSelected = 0x80
    };


    enum class HitTestZone : int {
        None = 0x0000,
        Client = 0x0001,
        SysMenu = 0x0002,
        CaptionBar = 0x0004,

        MinButton = 0x0010,
        MaxButton = 0x0020,
        CloseButton = 0x0040,

        Left = 0x0100,
        Top = 0x0200,
        Right = 0x0400,
        Bottom = 0x0800,

        TopLeft = Top | Left,
        TopRight = Top | Right,
        BottomLeft = Bottom | Left,
        BottomRight = Bottom | Right
    };

    enum ResourceType : uint32_t {
        Binary = 0x00,
        Image = 0x01,
        Gif = 0x02,
        NinePatchImage = 0x03,
        Cursor = 0x04,
        Xml = 0x05,
        CompiledXml = 0x06,
    };


    enum SystemCursorType {
        SmallWaiting,
        LargeWaiting,
        Arrow,
        Cross,
        Hand,
        Help,
        IBeam,
        SizeAll,
        SizeNESW,
        SizeNS,
        SizeNWSE,
        SizeWE,
    };


    enum GraphicsSerializableType : uint16_t {
        t_Attributes = SerializableType::t_CustomizeData + 1,
        t_ElementNode,

        t_GraphicsCustomizedData = t_ElementNode + 1000,
    };


    class STRAW_FRAMEWORK_API View;

    class ViewUpdateLock {
    public:
        enum class DoWhenUnlock {
            Nothing,
            ReLayout,
            Invalidate
        };

        ViewUpdateLock(View* view, DoWhenUnlock whenUnlock = DoWhenUnlock::ReLayout);
        ~ViewUpdateLock();

    private:
        View* mView;
        DoWhenUnlock mDoWhenUnlock;
    };


    class STRAW_FRAMEWORK_API Object {
    };


    template <typename This, typename Parent>
    class InheritTraits : public Parent {
    public:
        template <typename... T>
        InheritTraits(T& ... args) : Parent(args...) {
        }

    public:
        using super = Parent;
        using this_class = This;
    };


    namespace extension {
        const char* const PNG = ".png";
        const char* const JPG = ".jpg";
        const char* const BMP = ".bmp";
        const char* const JPEG = ".jpeg";
        const char* const GIF = ".gif";
        const char* const ICO = ".ico";
        const char* const WEBP = ".webp";
        const char* const NINE_PATCH_PNG = ".9.png";
        const char* const CURSOR = ".cur";
        const char* const XML = ".xml";
    }


    namespace layout {

        const char* const attr_width = "width";
        const char* const attr_height = "height";
        const char* const attr_position = "position";

        const char* const attr_extensionName = "extensionName";
        const char* const attr_style = "style";
        const char* const attr_layout = "layout";

        const char* const val_null = "null";
    }


    namespace window {
        const char* const id_wndCaptionRoot = "caption_root";
        const char* const id_wndContentRoot = "content_root";
        const char* const id_wndRootView = "window_root_view";

        const char* const id_wndIcon = "wnd_icon";
        const char* const id_wndTitle = "wnd_title";
        const char* const id_wndMinBtn = "wnd_min_btn";
        const char* const id_wndMaxBtn = "wnd_max_btn";
        const char* const id_wndCloseBtn = "wnd_close_btn";

        const char* const attr_wndCaptionLayout = "captionLayout";
        const char* const attr_wndCaptionStyle = "captionStyle";
        const char* const attr_wndClientBackground = "clientBackground";

        const char* const attr_minBtnDrawable = "minBtnDrawable";
        const char* const attr_maxBtnDrawable = "maxBtnDrawable";
        const char* const attr_closeBtnDrawable = "closeBtnDrawable";
        const char* const attr_restoreBtnDrawable = "restoreBtnDrawable";
    }


    namespace menu {

        const char* const id_menuIcon = "menu_icon";
        const char* const id_menuTitle = "menu_title";
        const char* const id_menuGrayTips = "menu_gray_tips";
        const char* const id_menuSubIcon = "menu_sub_icon";

        const char* const attr_isDivider = "isDivider";
    }


    namespace config {

        const char* const APP_NAME = "app";

        const char* const PLATFORM_RESOURCES = "platform_resources";
        const char* const APPLICATION_RESOURCES = "resources";
        const char* const RDB_FILE_EXTENSION = ".rdb";
        const char* const PLUGIN_ROOT_DIRECTORY = "plugins";
        const char* const PLUGIN_CONFIG_FILE = "plugin_config.dat";

        const char* const RESOURCE_ROOT_TAG = "resources";
        const char* const RESOURCE_ITEM_TAG = "item";
        const char* const RESOURCE_NAME_ATTR = "name";
        const char* const RESOURCE_PARENT_ATTR = "parent";
        const char* const RESOURCE_TARGET_ATTR = "target";

        const std::string EMPTY_STRING = "";
    }


#ifdef STRAW_BUILD_FOR_WIN32
    const char* const DIRECTORY_SEPARATOR = "\\";
    const char* const PLUGIN_LIBRARY_EXTENSION = ".dll";
#else
    const char* const DIRECTORY_SEPARATOR = "/";
    const char* const PLUGIN_LIBRARY_EXTENSION = ".so";
#endif // STRAW_BUILD_FOR_WIN32


#ifdef DEBUG
    const char* const BIN_DIRECTORY_NAME = "bindebug";
#else
    const char* const BIN_DIRECTORY_NAME = "bin";
#endif // DEBUG


    STRAW_FRAMEWORK_API bool isImageExtension(const std::string& extension);


}
