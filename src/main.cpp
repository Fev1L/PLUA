#include "../include/Application.h"

#include <sqlite3.h>

int main()
{
    Application app;

    if (!app.Init())
    {
        return -1;
    }

    app.Run();
    app.Shutdown();

    return 0;
}