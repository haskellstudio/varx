/*
  ==============================================================================

    ReactiveTest.cpp
    Created: 28 Apr 2017 8:12:02pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Reactive.h"
#include "rxjuce_VariantConverters.h"


TEST_CASE("Reactive<Value> conversion",
		  "[Reactive<Value>][ValueExtension]")
{
	Reactive<Value> value;
	
	IT("supports copy assignment from var-compatible types") {
		value = 3;
		value = Array<var>({6, 7, 5});
		value = "Some String";
		REQUIRE(value.getValue() == "Some String");
	}
	
	IT("can be implicitly converted to var") {
		value.setValue("Testing");
		var v = value;
		REQUIRE(v == "Testing");
	}
	
	IT("supports == and != with var-compatible types") {
		value.setValue("Hello!");
		REQUIRE(value == "Hello!");
		REQUIRE(value != "World");
		REQUIRE(value != 3.45);
		REQUIRE(value != 2);
	}
}


TEST_CASE("Reactive<Value> Observable",
		  "[Reactive<Value>][ValueExtension]")
{
	auto value = std::make_shared<Reactive<Value>>("Initial");
	Array<var> items;
	RxJUCECollectItems(value->rx.subject, items);
	
	IT("emits items asynchronously when the Value changes") {
		value->setValue("Second");
		RxJUCECheckItems(items, "Initial");
		RxJUCERunDispatchLoop();
		RxJUCECheckItems(items, "Initial", "Second");
		value->setValue("Third");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial", "Second", "Third");
	}
	
	IT("stops emitting items immediately when being destroyed") {
		value->setValue("Should not arrive");
		value.reset();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial");
	}
}

TEST_CASE("Reactive<Component>",
		  "[Reactive<Component>][ComponentExtension]")
{
	Array<var> items;
	Reactive<Component> component;
	RxJUCECollectItems(component.rx.visible, items);
	
	IT("initially has the same value as the getter") {
		REQUIRE(component.isVisible() == fromVar<bool>(component.rx.visible.getLatestItem()));
	}
	
	IT("emits when visibility is changed through setter") {
		for (bool visible : {false, false, true, true, false}) {
			component.setVisible(visible);
		}
		
		RxJUCERequireItems(items, false, true, false);
	}
	
	IT("changes visiblility when pushing items") {
		for (bool visible : {false, false, true, true, false}) {
			component.rx.visible.onNext(visible);
			
			REQUIRE(component.isVisible() == visible);
		}
	}
}


TEST_CASE("Reactive<ImageComponent>",
		  "[Reactive<ImageComponent>][ImageComponentExtension]")
{
	Reactive<ImageComponent> imageComponent;
	const Image image1(Image::RGB, 17, 47, false);
	const Image image2(Image::RGB, 32, 12, false);
	const RectanglePlacement placement(RectanglePlacement::onlyReduceInSize | RectanglePlacement::yBottom);
	
	IT("allows setting a new image and placement") {
		imageComponent.rx.image.onNext(toVar(image1));
		
		CHECK(imageComponent.getImage().getWidth() == 17);
		CHECK(imageComponent.getImage().getHeight() == 47);
		
		imageComponent.rx.imagePlacement.onNext(toVar(placement));
		
		CHECK(imageComponent.getImagePlacement() == placement);
		
		imageComponent.rx.image.onNext(toVar(image2));
		
		REQUIRE(imageComponent.getImage().getWidth() == 32);
		REQUIRE(imageComponent.getImage().getHeight() == 12);
		
		REQUIRE(imageComponent.getImagePlacement() == placement);
	}
}


TEST_CASE("Reactive<Button> buttonState",
		  "[Reactive<Button>][ButtonExtension]")
{
	Reactive<TextButton> button("Click Here");
	Array<var> items;
	RxJUCECollectItems(button.rx.buttonState, items);
	
	IT("emits the normal state on subscribe") {
		RxJUCERequireItems(items, Button::ButtonState::buttonNormal);
	}
	
	IT("emits items synchronously when the Button state changes") {
		button.setState(Button::ButtonState::buttonDown);
		
		RxJUCECheckItems(items,
						 Button::ButtonState::buttonNormal,
						 Button::ButtonState::buttonDown);
		
		button.setState(Button::ButtonState::buttonNormal);
		button.setState(Button::ButtonState::buttonOver);
		
		RxJUCERequireItems(items,
						   Button::ButtonState::buttonNormal,
						   Button::ButtonState::buttonDown,
						   Button::ButtonState::buttonNormal,
						   Button::ButtonState::buttonOver);
	}
}


TEST_CASE("Reactive<Button> clicked",
		  "[Reactive<Button>][ButtonExtension]")
{
	Reactive<TextButton> button("Click Here");
	Array<var> items;
	RxJUCECollectItems(button.rx.clicked, items);
	
	IT("doesn't emit an item on subscribe") {
		REQUIRE(items.isEmpty());
	}
	
	IT("emits void vars asynchronously when the Button is clicked") {
		button.triggerClick();
		RxJUCERunDispatchLoop();
		
		RxJUCECheckItems(items, var::undefined());
		
		button.triggerClick();
		button.triggerClick();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, var::undefined(), var::undefined(), var::undefined());
	}
}


TEST_CASE("Reactive<Button> with custom TextButton subclass",
		  "[Reactive<Button>][ButtonExtension]")
{
	class MyButton : public TextButton
	{
	public:
		void hoverAcrossButton()
		{
			setState(buttonOver);
			MessageManager::getInstance()->callAsync([this]() {
				setState(buttonNormal);
			});
		}
	};
	
	Reactive<MyButton> button;
	Array<var> items;
	RxJUCECollectItems(button.rx.buttonState, items);
	
	IT("initially has the normal state") {
		RxJUCERequireItems(items, Button::ButtonState::buttonNormal);
	}
	
	IT("changes states when calling the method in the custom subclass") {
		button.hoverAcrossButton();
		RxJUCECheckItems(items,
						 Button::ButtonState::buttonNormal,
						 Button::ButtonState::buttonOver);
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items,
						   Button::ButtonState::buttonNormal,
						   Button::ButtonState::buttonOver,
						   Button::ButtonState::buttonNormal);
	}
}


TEST_CASE("Reactive<Label>",
		  "[Reactive<Label>][LabelExtension]")
{
	Reactive<Label> label;
	Array<var> items;
	
	CONTEXT("text") {
		RxJUCECollectItems(label.rx.text, items);
		
		IT("initially emits the empty String") {
			CHECK(label.getText().isEmpty());
			
			RxJUCERequireItems(items, label.getText());
		}
		
		IT("emits a items when the Label changes its text") {
			label.setText("Foo", sendNotificationSync);
			label.setText("Bar", sendNotificationSync);
			
			RxJUCERequireItems(items, "", "Foo", "Bar");
		}
		
		IT("changes the Label text synchronously when calling onNext") {
			for (auto text : {"Hello", "World!"}) {
				label.rx.text.onNext(text);
			}
			
			RxJUCERequireItems(items, "", "Hello", "World!");
		}
	}
	
	CONTEXT("showEditor, discardChangesWhenHidingEditor and textEditor") {
		DisposeBag disposeBag;
		RxJUCECollectItems(label.rx.showEditor, items);
		
		Array<Component *> editors;
		label.rx.textEditor.subscribe([&](var editor) {
			editors.add(fromVar<WeakReference<Component>>(editor));
		}).disposedBy(disposeBag);
		
		// The label must be on the screen to show an editor (asserts otherwise)
		TestWindow::getInstance().addAndMakeVisible(label);
		
		IT("is is initially false/nullptr") {
			CHECK(label.getCurrentTextEditor() == nullptr);
			CHECK(editors == Array<Component *>({nullptr}));
			
			RxJUCERequireItems(items, false);
		}
		
		IT("remains false if the discard setting is changed") {
			label.rx.discardChangesWhenHidingEditor.onNext(true);
			CHECK(label.getCurrentTextEditor() == nullptr);
			RxJUCECheckItems(items, false);
			CHECK(editors == Array<Component *>({nullptr}));
			
			label.rx.discardChangesWhenHidingEditor.onNext(false);
			REQUIRE(label.getCurrentTextEditor() == nullptr);
			CHECK(editors == Array<Component *>({nullptr}));
			
			RxJUCERequireItems(items, false);
		}
		
		IT("becomes true/non-null when calling Label::showEditor") {
			label.showEditor();
			CHECK(label.getCurrentTextEditor() != nullptr);
			CHECK(editors.size() == 2);
			CHECK(editors.getLast() != nullptr);
			
			RxJUCERequireItems(items, false, true);
			
			IT("becomes false/nullptr when calling Label::hideEditor") {
				label.hideEditor(true);
				CHECK(label.getCurrentTextEditor() == nullptr);
				CHECK(editors.size() == 3);
				CHECK(editors.getLast() == nullptr);
				
				RxJUCERequireItems(items, false, true, false);
			}
		}
		
		IT("shows the editor when pushing true") {
			label.rx.showEditor.onNext(true);
			RxJUCECheckItems(items, false, true);
			CHECK(editors.size() == 2);
			CHECK(editors.getLast() != nullptr);
			
			REQUIRE(label.getCurrentTextEditor() != nullptr);
			
			IT("keeps showing the editor even if the discard setting changes") {
				label.rx.discardChangesWhenHidingEditor.onNext(false);
				label.rx.discardChangesWhenHidingEditor.onNext(true);
				RxJUCECheckItems(items, false, true);
				CHECK(editors.size() == 2);
				CHECK(editors.getLast() != nullptr);
				
				REQUIRE(label.getCurrentTextEditor() != nullptr);
			}
			
			IT("hides the editor when pushing false") {
				label.rx.showEditor.onNext(false);
				RxJUCECheckItems(items, false, true, false);
				CHECK(editors.size() == 3);
				CHECK(editors.getLast() == nullptr);
				
				REQUIRE(label.getCurrentTextEditor() == nullptr);
			}
		}
	}
	
	CONTEXT("font") {
		const Font font1(18.43, Font::bold | Font::underlined);
		const Font font2(4.3, Font::italic);
		
		IT("changes the Label font when pushing items") {
			label.rx.font.onNext(toVar(font1));
			CHECK(label.getFont() == font1);
			label.rx.font.onNext(toVar(font2));
			
			REQUIRE(label.getFont() == font2);
		}
	}
	
	CONTEXT("justificationType") {
		const Justification justification1(Justification::horizontallyJustified | Justification::top);
		const Justification justification2(Justification::bottom | Justification::left);
		
		IT("changes the justification type when pushing items") {
			label.rx.justificationType.onNext(toVar(justification1));
			CHECK(label.getJustificationType() == justification1);
			label.rx.justificationType.onNext(toVar(justification2));
			
			REQUIRE(label.getJustificationType() == justification2);
		}
	}
	
	CONTEXT("borderSize") {
		const BorderSize<int> borderSize1(1, 5, 8, 2);
		const BorderSize<int> borderSize2(33, 108, 47, 0);
		
		IT("changes the border size when pushing items") {
			label.rx.borderSize.onNext(toVar(borderSize1));
			CHECK(label.getBorderSize() == borderSize1);
			label.rx.borderSize.onNext(toVar(borderSize2));
			
			REQUIRE(label.getBorderSize() == borderSize2);
		}
	}
	
	CONTEXT("attachedComponent") {
		label.attachToComponent(nullptr, false);
		CHECK(label.getAttachedComponent() == nullptr);
		CHECK(label.isAttachedOnLeft() == false);
		
		IT("attaches to another Component") {
			Component other;
			label.rx.attachedComponent.onNext(toVar(WeakReference<Component>(&other)));
			
			CHECK(label.isAttachedOnLeft() == false);
			REQUIRE(label.getAttachedComponent() == &other);
			
			IT("can change the attachment side and keeps the Component") {
				label.rx.attachedOnLeft.onNext(true);
				label.rx.attachedOnLeft.onNext(false);
				label.rx.attachedOnLeft.onNext(true);
				
				CHECK(label.isAttachedOnLeft() == true);
				REQUIRE(label.getAttachedComponent() == &other);
			}
			
			IT("can remove the attachment again via var::undefined()") {
				label.rx.attachedComponent.onNext(var::undefined());
				
				REQUIRE(label.getAttachedComponent() == nullptr);
			}
			
			IT("can remove the attachment again via var()") {
				label.rx.attachedComponent.onNext(var());
				
				REQUIRE(label.getAttachedComponent() == nullptr);
			}
			
			IT("can remove the attachment again via an empty weak reference") {
				label.rx.attachedComponent.onNext(toVar(WeakReference<Component>()));
				
				REQUIRE(label.getAttachedComponent() == nullptr);
			}
		}
		
		IT("loses the attachment when the other component is destroyed") {
			auto other = std::make_shared<Component>();
			label.rx.attachedComponent.onNext(toVar(WeakReference<Component>(other.get())));
			CHECK(label.getAttachedComponent() == other.get());
			
			other.reset();
			
			REQUIRE(label.getAttachedComponent() == nullptr);
		}
	}
	
	CONTEXT("minimumHorizontalScale") {
		IT("changes the scale when pushing items") {
			CHECK(label.getMinimumHorizontalScale() == 0);
			
			for (auto scale : {0.f, 15.f, 0.33f, 0.f, 4.24f}) {
				label.rx.minimumHorizontalScale.onNext(scale);
				REQUIRE(label.getMinimumHorizontalScale() == scale);
			}
		}
	}
	
	CONTEXT("keyboardType") {
		// The label must be on the screen to show an editor (asserts otherwise)
		TestWindow::getInstance().addAndMakeVisible(label);
		
		IT("changes the keyboard type when pushing items") {
			label.rx.keyboardType.onNext(toVar(TextInputTarget::emailAddressKeyboard));
			label.showEditor();
			CHECK(label.getCurrentTextEditor() != nullptr);
			
			REQUIRE(label.getCurrentTextEditor()->getKeyboardType() == TextInputTarget::emailAddressKeyboard);
			
			IT("can change the type while an editor is showing") {
				label.rx.keyboardType.onNext(toVar(TextInputTarget::decimalKeyboard));
				
				REQUIRE(label.getCurrentTextEditor()->getKeyboardType() == TextInputTarget::decimalKeyboard);
			}
			
			IT("remembers the type when hiding and showing again") {
				label.hideEditor(true);
				CHECK(label.getCurrentTextEditor() == nullptr);
				label.showEditor();
				
				REQUIRE(label.getCurrentTextEditor()->getKeyboardType() == TextInputTarget::emailAddressKeyboard);
			}
		}
	}
	
	CONTEXT("editable") {
		CHECK(!label.isEditableOnSingleClick());
		CHECK(!label.isEditableOnDoubleClick());
		CHECK(!label.doesLossOfFocusDiscardChanges());
		
		IT("changes only single click when pushing to single click") {
			label.rx.editableOnSingleClick.onNext(true);
			REQUIRE(label.isEditableOnSingleClick());
			
			CHECK(!label.isEditableOnDoubleClick());
			CHECK(!label.doesLossOfFocusDiscardChanges());
			
			label.rx.editableOnSingleClick.onNext(false);
			REQUIRE(!label.isEditableOnSingleClick());
		}
		
		IT("changes only double click when pushing to double click") {
			label.rx.editableOnDoubleClick.onNext(true);
			REQUIRE(label.isEditableOnDoubleClick());
			
			CHECK(!label.isEditableOnSingleClick());
			CHECK(!label.doesLossOfFocusDiscardChanges());
			
			label.rx.editableOnDoubleClick.onNext(false);
			REQUIRE(!label.isEditableOnDoubleClick());
		}
		
		IT("changes only the discard setting when pushing to it") {
			label.rx.lossOfFocusDiscardsChanges.onNext(true);
			REQUIRE(label.doesLossOfFocusDiscardChanges());
			
			CHECK(!label.isEditableOnSingleClick());
			CHECK(!label.isEditableOnDoubleClick());
			
			label.rx.lossOfFocusDiscardsChanges.onNext(false);
			REQUIRE(!label.doesLossOfFocusDiscardChanges());
		}
	}
}


template<typename T1, typename T2>
using isSame = typename std::is_same<typename std::decay<T1>::type, T2>;

TEST_CASE("Template ambiguities")
{
	Array<var> items;
	
	IT("chooses the correct template for a juce::Component") {
		Reactive<Component> myComponent;
		static_assert(isSame<decltype(myComponent.rx), ComponentExtension>::value, "rx Member has wrong type.");
	}
	
	IT("chooses the correct template for a Component subclass") {
		class MyCustomComponent : public Component {};
		Reactive<MyCustomComponent> myCustomComponent;
		static_assert(isSame<decltype(myCustomComponent.rx), ComponentExtension>::value, "rx Member has wrong type.");
	}
	
	IT("chooses the correct template for a Button subclass") {
		class MyButton : public Button {
		public:
			MyButton()
			: Button("") {}
			void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) override {}
		};
		Reactive<MyButton> myButton;
		static_assert(isSame<decltype(myButton.rx), ButtonExtension>::value, "rx Member has wrong type.");
	}
	
	IT("chooses the correct template for a juce::ImageComponent") {
		Reactive<ImageComponent> myImageComponent;
		static_assert(isSame<decltype(myImageComponent.rx), ImageComponentExtension>::value, "rx Member has wrong type.");
	}
}
