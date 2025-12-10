#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// ---------------- Display Pins (Wokwi Standard) ----------------
#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

class Ball
{
    public:
    float X, Y, speedX, speedY;
    int color, size;

    Ball(float x=0, float y=SCREEN_HEIGHT/2, float s_x=2, float s_y=0, int sz=6, int c = ILI9341_CYAN)
    {
        X = x;
        Y = y;
        speedX = s_x;
        speedY = s_y;
        size = sz;
        color = c;
    }

    void erase()
    {
        tft.fillCircle(X, Y, size, ILI9341_BLACK);
    }

    void draw()
    {
       tft.fillCircle(X, Y, size, color);
    }

    void update()
    {
        erase();
        X += speedX;
        Y += speedY;
        draw();
    }
};

Ball ball1(20, 20, 5, 5, 15);
Ball ball2(210, 210, 0, 0, 15);
int size1 = sq(ball1.size);
int size2 = sq(ball2.size);

void setup()
{
    Serial.begin(115200);

    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
}

void loop()
{
    // Wall collisions
    if (ball1.X <= ball1.size || ball1.X >= SCREEN_WIDTH - ball1.size) ball1.speedX = -ball1.speedX;
    if (ball1.Y <= ball1.size || ball1.Y >= SCREEN_HEIGHT - ball1.size) ball1.speedY = -ball1.speedY;
    if (ball2.X <= ball2.size || ball2.X >= SCREEN_WIDTH - ball2.size) ball2.speedX = -ball2.speedX;
    if (ball2.Y <= ball2.size || ball2.Y >= SCREEN_HEIGHT - ball2.size) ball2.speedY = -ball2.speedY;
    // Main Collision Detection
    float dist = sqrt(sq(ball1.X-ball2.X)+sq(ball1.Y-ball2.Y));
    if (dist<=ball1.size+ball2.size){
        
        float angle = atan2(ball2.Y-ball1.Y,ball2.X-ball1.X);
        float Up1 = ball1.speedX*sin(angle)-ball1.speedY*cos(angle);
        float Ua1 = ball1.speedX*cos(angle)+ball1.speedY*sin(angle);
        float Up2 =ball2.speedX*sin(angle)-ball2.speedY*cos(angle);
        float Ua2 = ball2.speedX*cos(angle)+ball2.speedY*sin(angle);

        float Va2 = 2*(size1)*Ua1/(size1+size2) - (size1-size2)*Ua2/(size1+size2);
        float Va1 = (size1-size2)*Ua1/(size1+size2) + 2*(size2)*Ua2/(size1+size2);
        ball1.speedX = int(Va1*cos(angle)+Up1*sin(angle));
        ball1.speedY =  int(Va1*sin(angle)-Up1*cos(angle));
        ball2.speedX = int(Va2*cos(angle)+Up2*sin(angle));
        ball2.speedY =int(Va2*sin(angle)-Up2*cos(angle));

    }

    
    
    ball1.update();
    ball2.update();
    delay(8);
}