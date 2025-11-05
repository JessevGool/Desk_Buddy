// Will use the rectangle function to draw the progress bar
#include <Adafruit_ILI9341.h>

namespace DeskBuddy
{

    class ProgressBar
    {
    public:
        ProgressBar(Adafruit_ILI9341& tft, int x, int y, int width, int height);
        ProgressBar(Adafruit_ILI9341& tft, int x, int y, int width, int height, int progress);
        void setProgress(int progress);
        void draw();

    private:
        Adafruit_ILI9341& tft;
        int x, y, width, height;
        int progress;
    };

}