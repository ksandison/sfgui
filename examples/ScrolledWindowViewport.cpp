#include <cstdlib>

#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// Including SFGUI/SFGUI.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>

class ScrolledWindowViewportExample {
	public:
		// Our button click handler.
		void OnButtonClick();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		// Create the ScrolledWindow box pointer here to reach it from OnButtonClick().
		sfg::Box::Ptr m_scrolled_window_box;
};

void ScrolledWindowViewportExample::OnButtonClick() {
	m_scrolled_window_box->Pack( sfg::Button::Create( "A Button" ) );
}

void ScrolledWindowViewportExample::Run() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI ScrolledWindow and Viewport Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create a box with 10 pixel spacing.
	sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::VERTICAL, 10.f );

	// Create a button and connect the click signal.
	sfg::Button::Ptr button = sfg::Button::Create( "Add a button" );
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ScrolledWindowViewportExample::OnButtonClick, this );

	// Create a box for our ScrolledWindow. ScrolledWindows are bins
	// and can only contain 1 child widget.
	m_scrolled_window_box = sfg::Box::Create( sfg::Box::VERTICAL );

	// Create the ScrolledWindow.
	sfg::ScrolledWindow::Ptr scrolledwindow = sfg::ScrolledWindow::Create();

	// Set the ScrolledWindow to always show the horizontal scrollbar
	// and only show the vertical scrollbar when needed.
	scrolledwindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );

	// Add the ScrolledWindow box to the ScrolledWindow
	// and create a new viewport automatically.
	scrolledwindow->AddWithViewport( m_scrolled_window_box );

	// Always remember to set the minimum size of a ScrolledWindow.
	scrolledwindow->SetRequisition( sf::Vector2f( 500.f, 100.f ) );

	// Create a viewport.
	sfg::Viewport::Ptr viewport = sfg::Viewport::Create();

	// Always remember to set the minimum size of a Viewport.
	viewport->SetRequisition( sf::Vector2f( 500.f, 200.f ) );

	// Set the vertical adjustment values of the Viewport.
	viewport->GetVerticalAdjustment()->SetLower( 0.f );
	viewport->GetVerticalAdjustment()->SetUpper( 100000000.f );

	// Create a box for our Viewport. Viewports are bins
	// as well and can only contain 1 child widget.
	sfg::Box::Ptr viewport_box = sfg::Box::Create( sfg::Box::VERTICAL );

	sf::err() << "Generating random strings, please be patient..." << std::endl;

	// Create some random text.
	for( int i = 0; i < 200; i++ ) {
		std::string str;

		for( int j = 0; j < 20; j++ ) {
			str += static_cast<char>( 65 + rand() % 26 );
		}

		// Add the text to the Viewport box.
		viewport_box->Pack( sfg::Label::Create( str.c_str() ) );
	}

	// Add the Viewport box to the viewport.
	viewport->Add( viewport_box );

	// Add everything to our box.
	box->Pack( button, false, true );
	box->Pack( scrolledwindow, false, true );
	box->Pack( viewport, true, true );

	// Add the box to the window.
	window->Add( box );

	sf::Clock clock;

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI every 1ms
		if( clock.getElapsedTime().asMicroseconds() >= 1000 ) {
			float delta = static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f;

			// Update() takes the elapsed time in seconds.
			window->Update( delta );

			// Add a nice automatic scrolling effect to our viewport ;)
			viewport->GetVerticalAdjustment()->SetValue( viewport->GetVerticalAdjustment()->GetValue() + delta * 10.f );

			clock.restart();
		}

		// Clear screen
		app_window.clear();

		// Draw the GUI
		m_sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}
}

int main() {
	ScrolledWindowViewportExample example;
	example.Run();

	return 0;
}
