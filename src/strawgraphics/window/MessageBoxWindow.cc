#include "graphics/window/MessageBoxWindow.h"

#include "graphics/view/Attributes.h"
#include "graphics/context/Context.h"
#include "graphics/window/MessageBox.h"
#include "graphics/view/Button.h"
#include "graphics/view/TextView.h"
#include "graphics/view/ImageView.h"


namespace {

    std::string getIconRes(sf::msgbox::IconType type) {
        std::string iconRes = "@platform:drawable/msgbox/";
        switch (type) {
        case sf::msgbox::None:
            break;
        case sf::msgbox::Information:
            iconRes += "icon_information";
            break;
        case sf::msgbox::Question:
            iconRes += "icon_question";
            break;
        case sf::msgbox::Warning:
            iconRes += "icon_warning";
            break;
        case sf::msgbox::Error:
            iconRes += "icon_error";
            break;
        case sf::msgbox::OK:
            iconRes += "icon_ok";
            break;
        default:
            break;
        }

        return iconRes;
    }

} // anonymous namespace ends here.


sf::MessageBoxWindow::MessageBoxWindow(const Context& context)
    : ElementInherit(context) {
}

sf::MessageBoxWindow::~MessageBoxWindow() {
}

void sf::MessageBoxWindow::createInternal(Attributes* attrs) {
    CaptionWindow::createInternal(attrs);

    std::string msgBoxLayout;
    if (attrs) {
        msgBoxLayout = attrs->getRawStringAttr(msgbox::layout::attr_msgBoxLayout);
    }

    if (msgBoxLayout.empty()) {
        msgBoxLayout = "@platform:layout/default_msg_box";
    }

    LayoutInflater& inflater = mContext.getLayoutInflater();
    inflater.inflate(msgBoxLayout, getContentParent(), true);
}

void sf::MessageBoxWindow::dismiss() {
    close();
}

void sf::MessageBoxWindow::setLeftBtnText(const std::string& text) {
    Button* left = reinterpret_cast<Button*>(findViewById(msgbox::layout::id_leftBtn));
    if (left) {
        left->setText(text);
    }
}

void sf::MessageBoxWindow::setRightBtnText(const std::string& text) {
    Button* right = reinterpret_cast<Button*>(findViewById(msgbox::layout::id_rightBtn));
    if (right) {
        right->setText(text);
    }
}

void sf::MessageBoxWindow::setLeftBtnClickListener(msgbox::MsgBoxBtnListener listener) {
    Button* left = reinterpret_cast<Button*>(findViewById(msgbox::layout::id_leftBtn));
    if (left) {
        left->addOnClickListener([this, listener](View*) -> void {
            listener(this);
        });
    }
}

void sf::MessageBoxWindow::setRightBtnClickListener(msgbox::MsgBoxBtnListener listener) {
    Button* right = reinterpret_cast<Button*>(findViewById(msgbox::layout::id_rightBtn));
    if (right) {
        right->addOnClickListener([this, listener](View*) -> void {
            listener(this);
        });
    }
}

void sf::MessageBoxWindow::setSingleBtn(bool single) {
    Button* left = reinterpret_cast<Button*>(findViewById(msgbox::layout::id_leftBtn));
    if (left) {
        left->setVisibility(single ? Visibility::Gone : Visibility::Visible);
    }
}

void sf::MessageBoxWindow::setIconType(msgbox::IconType iconType) {
    ImageView* icon = reinterpret_cast<ImageView*>(findViewById(msgbox::layout::id_msgIcon));
    if (icon) {
        icon->setBitmap(mContext.getBitmap(getIconRes(iconType)));
    }
}

void sf::MessageBoxWindow::setContentText(const std::string& content) {
    Button* msgView = reinterpret_cast<Button*>(findViewById(msgbox::layout::id_msgContent));
    if (msgView) {
        msgView->setText(content);
    }
}
