#include "graphics/view/Element.h"

#include "graphics/view/Button.h"
#include "graphics/view/TextView.h"
#include "graphics/view/ImageView.h"
#include "graphics/view/ListView.h"
#include "graphics/view/GridView.h"
#include "graphics/view/TreeView.h"
#include "graphics/view/EditText.h"
#include "graphics/view/CheckBox.h"
#include "graphics/view/ComboBox.h"
#include "graphics/view/ScrollView.h"
#include "graphics/view/SeekBar.h"
#include "graphics/view/ProgressBar.h"
#include "graphics/view/ToggleButton.h"

#include "graphics/layout/FrameLayout.h"
#include "graphics/layout/LinearLayout.h"
#include "graphics/layout/AbsoluteLayout.h"

#include "graphics/window/Window.h"
#include "graphics/window/CaptionWindow.h"
#include "graphics/menu/Menu.h"

#include "graphics/drawable/ShapeDrawable.h"
#include "graphics/drawable/ColorDrawable.h"
#include "graphics/drawable/BitmapDrawable.h"
#include "graphics/drawable/NinePatchDrawable.h"
#include "graphics/drawable/StateListDrawable.h"
#include "graphics/drawable/AnimationDrawable.h"

#include "graphics/color/StateListColor.h"

#include "graphics/context/Context.h"


DECLARE_ELEMENT(sf::Element);
DECLARE_ELEMENT(sf::View);

DECLARE_ELEMENT(sf::Button);
DECLARE_ELEMENT(sf::TextView);
DECLARE_ELEMENT(sf::ImageView);
DECLARE_ELEMENT(sf::ListView);
DECLARE_ELEMENT(sf::GridView);
DECLARE_ELEMENT(sf::TreeView);
DECLARE_ELEMENT(sf::ScrollView);
DECLARE_ELEMENT(sf::EditText);
DECLARE_ELEMENT(sf::CheckBox);
DECLARE_ELEMENT(sf::ComboBox);
DECLARE_ELEMENT(sf::SeekBar);
DECLARE_ELEMENT(sf::ProgressBar);
DECLARE_ELEMENT(sf::ToggleButton);

DECLARE_ELEMENT(sf::FrameLayout);
DECLARE_ELEMENT(sf::LinearLayout);
DECLARE_ELEMENT(sf::AbsoluteLayout);

DECLARE_ELEMENT(sf::Window);
DECLARE_ELEMENT(sf::CaptionWindow);
DECLARE_ELEMENT(sf::Menu);

DECLARE_ELEMENT(sf::ColorDrawable);
DECLARE_ELEMENT(sf::BitmapDrawable);
DECLARE_ELEMENT(sf::NinePatchDrawable);

DECLARE_ELEMENT_NAME(shape, sf::ShapeDrawable)
DECLARE_ELEMENT_NAME(animation-list, sf::AnimationDrawable)
DECLARE_ELEMENT_NAME(selector, sf::StateListDrawable);

DECLARE_ELEMENT_NAME(color, sf::StateListColor);
