#include "graphics/app/DeclareHelper.h"
#include "graphics/view/View.h"
#include "graphics/view/TextView.h"
#include "graphics/view/ImageView.h"
#include "graphics/window/BaseWindow.h"
#include "graphics/drawable/ShapeDrawable.h"


// Visibility
//
DECLARE_ENUM(sf::Visibility::Visible);
DECLARE_ENUM(sf::Visibility::Invisible);
DECLARE_ENUM(sf::Visibility::Gone);


// Gravity
//
DECLARE_ENUM(sf::Gravity::Left);
DECLARE_ENUM(sf::Gravity::Top);
DECLARE_ENUM(sf::Gravity::Right);
DECLARE_ENUM(sf::Gravity::Bottom);

DECLARE_ENUM(sf::Gravity::CenterHorizontal);
DECLARE_ENUM(sf::Gravity::CenterVertical);
DECLARE_ENUM(sf::Gravity::Center);

DECLARE_ENUM(sf::Gravity::TopLeft);
DECLARE_ENUM(sf::Gravity::TopRight);
DECLARE_ENUM(sf::Gravity::BottomLeft);
DECLARE_ENUM(sf::Gravity::BottomRight);


// Orientation
//
DECLARE_ENUM(sf::Orientation::Horizontal);
DECLARE_ENUM(sf::Orientation::Vertical);


// ScaleType
//
DECLARE_ENUM(sf::ScaleType::FitXY);
DECLARE_ENUM(sf::ScaleType::FitStart);
DECLARE_ENUM(sf::ScaleType::FitEnd);
DECLARE_ENUM(sf::ScaleType::FitCenter);
DECLARE_ENUM(sf::ScaleType::InCenter);
DECLARE_ENUM(sf::ScaleType::CenterCrop);
DECLARE_ENUM(sf::ScaleType::CenterInside);


// EllipsizeType
//
DECLARE_ENUM(sf::EllipsizeType::NoEllipseize);
DECLARE_ENUM(sf::EllipsizeType::Start);
DECLARE_ENUM(sf::EllipsizeType::Middle);
DECLARE_ENUM(sf::EllipsizeType::End);


// TextStyle
//
DECLARE_ENUM(sf::TextStyle::Normal);
DECLARE_ENUM(sf::TextStyle::Italic);
DECLARE_ENUM(sf::TextStyle::Bold);
DECLARE_ENUM(sf::TextStyle::Underline);
DECLARE_ENUM(sf::TextStyle::Strike);


// SizeableType
//
DECLARE_ENUM(sf::SizeableType::Disabled);
DECLARE_ENUM(sf::SizeableType::VerticalSizeable);
DECLARE_ENUM(sf::SizeableType::HorizontalSizeable);
DECLARE_ENUM(sf::SizeableType::BothSizeable);


// ShapeType
//
DECLARE_ENUM(sf::ShapeType::Rectangle);
DECLARE_ENUM(sf::ShapeType::Oval);


// GradientType
//
DECLARE_ENUM(sf::GradientType::NoGradient);
DECLARE_ENUM(sf::GradientType::Linear);
DECLARE_ENUM(sf::GradientType::Radial);
DECLARE_ENUM(sf::GradientType::Sweep);


// SystemCursorType
//
DECLARE_ENUM(sf::SystemCursorType::SmallWaiting);
DECLARE_ENUM(sf::SystemCursorType::LargeWaiting);
DECLARE_ENUM(sf::SystemCursorType::Arrow);
DECLARE_ENUM(sf::SystemCursorType::Cross);
DECLARE_ENUM(sf::SystemCursorType::Hand);
DECLARE_ENUM(sf::SystemCursorType::Help);
DECLARE_ENUM(sf::SystemCursorType::IBeam);
DECLARE_ENUM(sf::SystemCursorType::SizeAll);
DECLARE_ENUM(sf::SystemCursorType::SizeNESW);
DECLARE_ENUM(sf::SystemCursorType::SizeNS);
DECLARE_ENUM(sf::SystemCursorType::SizeNWSE);
DECLARE_ENUM(sf::SystemCursorType::SizeWE);

