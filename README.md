# StrawFramework 简介 & 目标

### 0、目前阶段

`开发实验` 阶段：

- 接口未固定 —— 后续重构、修改可能性大；
- 部分功能未实现 —— 如 EditText、ScrollView 等功能性组件；
- Bug 多；

缓慢持续进行中。。。

### 1、简介

`StrawFramework` 是一款基于 `C++` 的 `GUI` 开发框架。此前一直是开发 PC 端软件，但近年来由于我受 `Android` 开发的影响比较深，我决定把 Android 中的一些开发思路和经验借鉴过来，用 C++ 开发一款类似的 GUI 开发框架。`UI Layout`、`I18N`、`Drawable`、`Resources Manager` 等等模式和思路借鉴复制，另外针对 PC 端的开发模式，有一定其他不同的处理。

### 2、目标

- 桌面跨平台 C++ GUI 库——目前只开发 Windows PC 端，但接口有预留；
- 借鉴 Android 开发中好的一些设计思路和模式；
- 采用 XML 的方式进行界面排布、Drawable 等等资源编写；
- 设计插件模式，让其他插件可以用 DLL / so 的方式接入进来；

### 3、实现 / TODO 列表

|Component - Feature|Desc|
|---|---|
|Base Component|Application/ MessageLoop / Log / Timer / Data ...|
|DPI-Compatible|px / dp / sp ...|
|Window / Dialog|CaptionWindow / Dialog / MessageBox ...|
|Built-in Views|View / EditText / LineraLayout / ScrollView ...|
|Theme|Style / Theme ...|
|Layout|LinearLayout / AbsoluteLayout / FrameLayout ...|
|Drawable|BitmapDrawable / ShapeDrawable / ColorDrawable / GifDrawable / AnimationDrawable / StateListDrawable / NinePatchDrawable ...|
|I18N||
|Resources Manager|Resource packer & read|
|Menu||
|Message / Event||
|Extension / Plugin||

### 4、thirdparty library

- [skia](https://github.com/google/skia)
- [tinyxml2](https://github.com/leethomason/tinyxml2)

### 5、一个简单的布局

- 长度单位默认大小单位为： dp；
- 和 Android 类似，可以使用 `@dimen` / `@string` / `@drawable` / `@layout` 来索引资源；
- 支持 Layout-XML `include`；
- 插件或者宿主 App 均可使用 `@platform:drawable/xxx` 类似的做法访问 StrawFramework 库中的资源；
- 插件可使用 `@app:drawable/xxx` 访问宿主 App 库中的资源；
- 使用类似 `style="@style/login_btn"` 来为 View 指定样式/主题；

```xml
<CaptionWindow
    width="@dimen/wnd_width"
    height="@dimen/wnd_height"
    minSize="400, 300"
    title="@string/app_name"
    cursor="@drawable/aero_link"
    sizeable="bothSizeable">
    
    <FrameLayout
        width="match_parent"
        height="match_parent"
        extensionName="com.straw.demo.main_caption_layout">
        
        <include layout="@layout/base_header" />

        <LinearLayout
            width="match_parent"
            height="match_parent"
            bottomMargin="30"
            extensionName="com.straw.demo.main_layout"
            gravity="centerHorizontal"
            padding="4"
            orientation="horizontal">
            
            <View
                width="78"
                height="36"
                topMargin="10"
                layoutGravity="centerVertical"
                background="@drawable/audio_anim_left"/>
                
            <View
                width="150"
                height="180"
                leftMargin="40"
                topMargin="20"
                background="@drawable/demo"/>
                
            <View
                width="150"
                height="250"
                layoutGravity="bottom"
                bottomMargin="30"
                leftMargin="10"
                clickable="true"
                background="@drawable/caption"/>
                
        </LinearLayout>
        
        <View
            width="80"
            height="80"
            layoutGravity="bottom"
            marginBottom="100"
            background="@drawable/jump"/>
        
        <TextView
            width="200"
            height="80"
            layoutGravity="bottomRight"
            textSize="30"
            textColor="@color/hello_text_color"
            gravity="center"
            textStyle="bold|italic|underline|strike"
            text="Hello World!"/>

        <View
            width="match_parent"
            height="20"
            layoutGravity="bottom"
            background="#c44"/>
            
        <Button
            style="@style/login_btn"
            id="login"
            width="120"
            height="48"
            marginLeft="20"
            layoutGravity="centerVertical|left"
            text="Login"
            background="@drawable/state_drawable"/>
            
        <Button
            id="logout"
            width="120"
            height="42"
            layoutGravity="centerVertical|left"
            marginTop="60"
            marginLeft="20"
            text="Logout"
            textColor="@color/state_color"
            gravity="center"
            background="@platform:drawable/default_btn/default_btn"/>
    </FrameLayout>
</CaptionWindow>
```

### 6、部分截图

![CaptionWindow](https://github.com/arnozhang/strawframework/blob/master/docs/screenshots/CaptionWindow.png?raw=true)

![MessageBox](https://github.com/arnozhang/strawframework/blob/master/docs/screenshots/MessageBox.png?raw=true)
