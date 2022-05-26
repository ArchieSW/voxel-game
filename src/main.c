#include "Application.h"

int main (void) {
    Application* app = create_application(1920, 1080, "Game");

    while(app->is_running) {
        update(app);
        render(app);
    }
    
    delete_application(app);
    return 0;
}