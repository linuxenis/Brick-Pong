#include <iostream>
#include <cmath>
#include "raylib.h"
#include <vector>

using namespace std;

Color Green = Color{ 40, 185, 155, 255 };
Color LightGreen = Color{ 130, 205, 185, 255 };
Color Yellow = Color{ 245, 215, 90, 255 };
Color Red = Color{ 205, 50, 0 , 255};
Color Gray = Color{ 32, 32, 32 , 255 };
Color LightGray = Color{ 125, 125, 125, 255 };
Color DarkGray{ 50, 50, 50, 255 };
Color DarkerGray{ 25, 25, 25, 255 };

int leftscore = 0;
int rightscore = 0;

class Menu
{
private:
    float ly, ry, centerx, centery;
    int fontsize, counter;
    bool dideja;
    void adjust(bool& p1, bool& p2)
    {
        if (p1 == 0)
            ly = centery + 2 * fontsize;
        if (p2 == 0)
            ry = centery + 2 * fontsize;
    }
public:
    Menu(): ly(0), ry(0), centerx(0), centery(0), fontsize(0), counter(0), dideja(true) {}
    Menu(float cx, float cy, int size): ly(cy), ry(cy), centerx(cx), centery(cy), fontsize(size), counter(0), dideja(true) {}
    ~Menu() {}
    void update(bool &p1, bool &p2)
    {
        if (dideja == true)
            counter++;
        else if (dideja == false)
            counter--;
        if (counter >= fontsize / 2 + 5)
            dideja = false;
        else if (counter <= 0)
            dideja = true;

        if (IsKeyDown(KEY_KP_ADD) && fontsize < 100)
        {
            fontsize++;
            adjust(p1, p2);
        }
        if (IsKeyDown(KEY_KP_SUBTRACT) && fontsize >= 12)
        {
            fontsize--;
            adjust(p1, p2);
        }
        if (IsKeyDown(KEY_S))
        {
            ly = centery + 2 * fontsize;
            p1 = 0;
        }
        if (IsKeyDown(KEY_W))
        {
            ly = centery;
            p1 = 1;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            ry = centery + 2 * fontsize;
            p2 = 0;
        }
        if (IsKeyDown(KEY_UP))
        {
            ry = centery;
            p2 = 1;
        }
    }
    void draw()
    {
        ClearBackground(BLACK);
        DrawLine(10 + 25 / 2, 0, 10 + 25 / 2, GetScreenHeight(), DarkerGray); //sitie ne dynamic, tiesiog del astetikos idejau last second
        DrawLine(GetScreenWidth() - 10 - 25 / 2, 0, GetScreenWidth() - 10 - 25 / 2, GetScreenHeight(), DarkerGray);
        DrawRectangleRounded(Rectangle{ 10, float(GetScreenHeight())/2 - 120/2, 25, 120}, 0.8, 0, DarkGray);
        DrawRectangleRounded(Rectangle{ float(GetScreenWidth()) - 25 - 10, float(GetScreenHeight()) / 2 - 120 / 2, 25, 120}, 0.8, 0, DarkGray);
        DrawRectangleLinesEx(Rectangle{ centerx - MeasureText("   VS   ", fontsize) / 2 - MeasureText("Human", fontsize) - fontsize / 5 * 2, centery - fontsize + fontsize / 10, float(MeasureText("Human", fontsize)) + fontsize / 5 * 4, float(fontsize) + fontsize / 5 * 4 }, fontsize / 5, WHITE);
        DrawRectangleLinesEx(Rectangle{ centerx + MeasureText("   VS   ", fontsize) / 2 - fontsize / 5 * 2, centery - fontsize + fontsize / 10, float(MeasureText("Human", fontsize)) + fontsize / 5 * 4, float(fontsize) + fontsize / 5 * 4 }, fontsize / 5, WHITE);
        DrawText("Press space to begin", centerx - MeasureText("Press space to begin", fontsize + counter / 5) / 2, GetScreenHeight() - fontsize - 40, fontsize + counter/5, WHITE);
        DrawText("   VS   ", centerx - MeasureText("   VS   ", fontsize) / 2, GetScreenHeight()/2 - fontsize / 2, fontsize, WHITE);
        DrawText("Human", centerx - MeasureText("   VS   ", fontsize) / 2 - MeasureText("Human", fontsize) , ly - fontsize / 2, fontsize, GREEN);
        DrawText(" CPU", centerx - MeasureText("   VS   ", fontsize) / 2 - MeasureText("Human", fontsize) + fontsize/10, ly - fontsize / 2 - fontsize * 2, fontsize, BLUE);
        DrawText("Human", centerx + MeasureText("   VS   ", fontsize) / 2, ry - fontsize / 2, fontsize, GREEN);
        DrawText(" CPU", centerx + MeasureText("   VS   ", fontsize) / 2 + fontsize/10, ry - fontsize / 2 - fontsize * 2, fontsize, BLUE);
    }
};

class Ball                  //ball
{
private:
    float x, y, speedx, speedy, rawspeed, initialspeed, carryx, carryy, visasx, visasy;
    int radius;
public:
    Ball(): x(0), y(0), speedx(0), speedy(0), rawspeed(0), carryx(0), carryy(0), radius(0), visasx(0), visasy(0), initialspeed(0) {}
    Ball(int rad, float spd): x(0), y(0), speedx(0), speedy(0), rawspeed(0), carryx(0), carryy(0), radius(rad), visasx(0), visasy(0), initialspeed(spd) {}
    ~Ball() {}

    void draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }
    void update()
    {
        //cout << speedx << " " << speedy << " " << rawspeed << endl;
        carryx = modf(carryx + speedx, &visasx);
        carryy = modf(carryy + speedy, &visasy);
        x = x + visasx;
        y = y + visasy;
        rawspeed = abs(speedx) + abs(speedy);
    }
    void reset(int pradeda, float Lwidth, float Rwidth)
    {
        speedx = 0;
        speedy = 0;
        carryx = 0;
        carryy = 0;
        y = GetScreenHeight() / 2;
        if (pradeda == 0)
            x = 10 + Lwidth + radius;
        else
            x = GetScreenWidth() - 10 - Rwidth - radius;
    }
    void paddlenudge(int x, float y) // paeina link paddle judamos krypties
    {
        speedy += y / 2;
        speedx = (rawspeed - abs(speedy)) * x;
    }
    void edgenudge(int xd, int yd) // paeina link kampo krypties
    {
        speedx = (speedx + rawspeed * xd / 2) / 2;
        speedy = (speedy + rawspeed * yd / 2) / 2;
    }
    void updatespeed(float x, float y)
    {
        speedx = initialspeed * x;
        speedy = initialspeed * y;
    }
    void operator +=(float x) // '+' cia reiskia kad dirbama su x asimi, '-' reiskia kad su y
    {
        speedx *= x;
        carryx *= x;
        //cout << "+=" << endl;
    }
    void operator -=(float y)
    {
        speedy *= y;
        carryy *= y;
        //cout << "-=" << endl;
    }
    void operator ~()
    {
        swap(speedx, speedy);
        swap(carryx, carryy);
    }
    float get_x()
    {
        return x;
    }
    float get_y()
    {
        return y;
    }
    float get_rawspeed()
    {
        return rawspeed;
    }
    float get_speedx()
    {
        return speedx;
    }
    float get_speedy()
    {
        return speedy;
    }
    float get_radius()
    {
        return radius;
    }
};

class Paddle                    //paddle
{
protected:
    bool arzaidejas;
    float x, y, width, height, speed;
    void limitmovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
    void AIupdate(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 < ball_y)
        {
            y = y + speed;
        }
    }
public:
    Paddle(): arzaidejas(true), x(0), y(0), width(0), height(0), speed(0) {}
    Paddle(float X, float WIDTH, float HEIGHT, float SPEED, bool ARZAIDEJAS): arzaidejas(ARZAIDEJAS), x(X), y(0), width(WIDTH), height(HEIGHT), speed(SPEED) {}
    ~Paddle() {}

    void reset()
    {
        y = GetScreenHeight() / 2 - height / 2;
    }
    void draw()
    {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
    }
    bool get_arzaidejas()
    {
        return arzaidejas;
    }
    float get_width()
    {
        return width;
    }
    float get_height()
    {
        return height;
    }
    float get_speed()
    {
        return speed;
    }
    float get_x()
    {
        return x;
    }
    float get_y()
    {
        return y;
    }
};

class LeftPaddle : public Paddle    //cia inheritence tikriausiai nereikia, tiesiog neradau kur naudot, tai tiesiog paddle klase suskaldziau
{
    using Paddle::Paddle;
public:
    void update(int ball_y)
    {
        if(arzaidejas == true)
        {
            if (IsKeyDown(KEY_W))
            {
                y = y - speed;
            }
            if (IsKeyDown(KEY_S))
            {
                y = y + speed;
            }
        }
        else
        {
            AIupdate(ball_y);
        }
        limitmovement();
    }
};

class RightPaddle : public Paddle
{
    using Paddle::Paddle;
public:
    void update(int ball_y)
    {
        if (arzaidejas == true)
        {
            if (IsKeyDown(KEY_UP))
            {
                y = y - speed;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                y = y + speed;
            }
        }
        else
        {
            AIupdate(ball_y);
        }
        limitmovement();
    }
};

class Bricks                 //bricks
{
private:
    float brickx[21], bricky[21], brickwidth, brickheight;
public:
    Bricks()
    {
        brickwidth = 0;
        brickheight = 0;
        for (int i = 0; i < 21; i++)
        {
            brickx[i] = 0;
            bricky[i] = 0;
        }
    }
    Bricks(int width, int height)
    {
        brickwidth = width;
        brickheight = height;
        for (int i = 0; i < 21; i++)
        {
            brickx[i] = 0;
            bricky[i] = 0;
        }
    }
    ~Bricks() {}
    void reset()
    {
        int counter = 0;
        int kuris = rand() % 3;
        switch (kuris)
        {
        case 0:     //'kamuoliai'
        {
            int prx = GetScreenWidth() / 2 - 1.5 * brickwidth;
            int pry = GetScreenHeight() / 2 - 5 * brickheight;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    brickx[counter] = prx;
                    bricky[counter] = pry;
                    pry += brickheight;
                    counter++;
                }
                pry += 2 * brickheight;
            }
            prx += brickwidth;
            pry = GetScreenHeight() / 2 - 5.5 * brickheight;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    brickx[counter] = prx;
                    bricky[counter] = pry;
                    pry += brickheight;
                    counter++;
                }
                pry += brickheight;
            }
            prx += brickwidth;
            pry = GetScreenHeight() / 2 - 5 * brickheight;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    brickx[counter] = prx;
                    bricky[counter] = pry;
                    pry += brickheight;
                    counter++;
                }
                pry += 2 * brickheight;
            }
        }
        break;
        case 1:     //siena
        {
            int prx = GetScreenWidth() / 2 - 3.5 * brickwidth;
            int pry = GetScreenHeight() / 2 - 5 * brickheight;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    brickx[counter] = prx;
                    bricky[counter] = pry;
                    pry += 2 * brickheight;
                    counter++;
                }
                pry += 3 * brickheight;
            }
            prx += 3 * brickwidth;
            pry = GetScreenHeight() / 2 - 6.5 * brickheight;
            for (int i = 0; i < 13; i++)
            {
                brickx[counter] = prx;
                bricky[counter] = pry;
                pry += brickheight;
                counter++;
            }
            prx += 3 * brickwidth;
            pry = GetScreenHeight() / 2 - 5 * brickheight;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    brickx[counter] = prx;
                    bricky[counter] = pry;
                    pry += 2 * brickheight;
                    counter++;
                }
                pry += 3 * brickheight;
            }
        }
        break;
        case 2:     //akis
        {
            int prx = GetScreenWidth() / 2 - 4.5 * brickwidth;
            int pry = GetScreenHeight() / 2 - 1.5 * brickheight;
            int tarpas = 2 * brickheight;
            for (int i = 0; i < 5; i++)
            {
                brickx[counter] = prx;
                bricky[counter] = pry;
                counter++;
                brickx[counter] = prx;
                bricky[counter] = pry + tarpas;
                counter++;
                tarpas += 2 * brickheight;
                prx += brickwidth;
                pry -= brickheight;
            }
            pry += 2 * brickheight;
            tarpas -= 4 * brickheight;
            for (int i = 0; i < 4; i++)
            {
                brickx[counter] = prx;
                bricky[counter] = pry;
                counter++;
                brickx[counter] = prx;
                bricky[counter] = pry + tarpas;
                counter++;
                tarpas -= 2 * brickheight;
                prx += brickwidth;
                pry += brickheight;
            }
            pry = GetScreenHeight() / 2 - 0.5 * brickheight;
            prx = GetScreenWidth() / 2 - 4.5 * brickwidth;
            tarpas = 4 * brickwidth;
            for (int i = 0; i < 3; i++)
            {
                brickx[counter] = prx;
                bricky[counter] = pry;
                counter++;
                prx = prx + tarpas;
            }
        }
        break;
        }
    }
    void operator -(int i) //'naikinu' plyta (nenorejau su vektoriais knistis)
    {
        brickx[i] = -1000;
    }
    void draw()
    {
        for (int i = 0; i < 21; i++)
        {
            DrawRectangle(brickx[i], bricky[i], 38, 68, Gray);
            DrawRectangle(brickx[i] + 4, bricky[i] + 4, 30, 60, Red);
        }
    }
    float get_brickwidth()
    {
        return brickwidth;
    }
    float get_brickheight()
    {
        return brickheight;
    }
    float get_brickx(int i)
    {
        return brickx[i];
    }
    float get_bricky(int i)
    {
        return bricky[i];
    }
};

class Laser
{
private:
    float counter, pridedamas, target, startx, starty, endx, endy, degree;
public:
    Laser(): startx(0), starty(0), endx(0), endy(0), degree(0), pridedamas(0), target(0), counter(0) {}
    Laser(float prid, float tar): startx(0), starty(0), endx(0), endy(0), degree(0), pridedamas(prid), target(tar), counter(0) {}
    ~Laser() {}
    void AI(bool &shooting)
    {
        degree += pridedamas * 0.5;
        counter += 0.5;
        if (counter >= target)
        {
            shooting = false;
            counter = 0;
        }
    }
    void Update(int pradeda, bool ar1, bool ar2, bool &shooting)
    {
        if (pradeda == 0)
        {
            if (ar1 == true)
            {
                counter++;
                if (IsKeyDown(KEY_W) && degree > -70)
                {
                    degree -= 0.5;
                }
                else if (IsKeyDown(KEY_S) && degree < 70)
                {
                    degree += 0.5;
                }
                if (IsKeyPressed(KEY_SPACE) && counter > 5)
                {
                    shooting = false;
                    counter = 0;
                }
            }
            else //cpu lazeris
            {
                AI(shooting);
            }
        }
        if (pradeda == 1)
        {
            if (ar2 == true)
            {
                counter++;
                if (IsKeyDown(KEY_UP) && degree < 250)
                {
                    degree += 0.5;
                }
                else if (IsKeyDown(KEY_DOWN) && degree > 110)
                {
                    degree -= 0.5;
                }
                if (IsKeyPressed(KEY_SPACE) && counter > 5)
                {
                    shooting = false;
                    counter = 0;
                }
            }
            else
            {
                AI(shooting);
            }
        }
        float pie = 2 * acos(0.0);
        endx = cos(degree*pie/180);
        endy = sin(degree*pie/180);
    }
    void reset(float ballx, int pradeda)
    {
        starty = GetScreenHeight() / 2;
        startx = ballx;
        int prid = rand() % 2;
        if (prid == 0)
            prid--;
        pridedamas = prid;
        target = rand() % 61 + 10;
        if (pradeda == 0)
            degree = 0;
        else
            degree = 180;
    }
    void Draw(float ballx)
    {
        DrawLine(startx, starty, endx * 200 + ballx, endy * 200 + GetScreenHeight() / 2, Red);
    }
    float direction(bool arx)
    {
        double modulator = abs(endx) + abs(endy);
        if(arx)
        return endx / modulator;
        else
        return endy / modulator;
    }
};

void reset(Ball &ball, Laser &laser, LeftPaddle &Lpaddle, RightPaddle &Rpaddle, Bricks &bricks, bool &shooting, int pradeda) //reset when scored
{
    ball.reset(pradeda, Lpaddle.get_width(), Rpaddle.get_width());
    Lpaddle.reset();
    Rpaddle.reset();
    bricks.reset();
    laser.reset(ball.get_x(), pradeda);
    shooting = true;
}
void checkcollisions(Ball &ball, RightPaddle &Rpaddle, Laser &laser, LeftPaddle &Lpaddle, Bricks &bricks, bool &shooting, int &pradeda)              //collisions
{
    if (CheckCollisionCircleRec(Vector2{ ball.get_x(), ball.get_y()}, ball.get_radius(), Rectangle{Rpaddle.get_x(), Rpaddle.get_y(), Rpaddle.get_width(), Rpaddle.get_height()}) && ball.get_speedx() > 0) //ball with right paddle
    {
        ball += -1;
        if (IsKeyDown(KEY_UP) && ball.get_speedy() - Rpaddle.get_speed() / 2 > -ball.get_rawspeed() + ball.get_rawspeed() / 5 && Rpaddle.get_arzaidejas())
        {
            ball.paddlenudge(-1, -Rpaddle.get_speed());
        }
        else if (IsKeyDown(KEY_DOWN) && ball.get_speedy() + Rpaddle.get_speed() / 2 < ball.get_rawspeed() - ball.get_rawspeed() / 5 && Rpaddle.get_arzaidejas())
        {
            ball.paddlenudge(-1, Rpaddle.get_speed());
        }
    }
    if (CheckCollisionCircleRec(Vector2{ ball.get_x(), ball.get_y()}, ball.get_radius(), Rectangle{Lpaddle.get_x(), Lpaddle.get_y(), Lpaddle.get_width(), Lpaddle.get_height()}) && ball.get_speedx() < 0) //ball with left paddle
    {
        ball += -1;
        if (IsKeyDown(KEY_W) && ball.get_speedy() - Lpaddle.get_speed() / 2 > -ball.get_rawspeed() + ball.get_rawspeed() / 5 && Lpaddle.get_arzaidejas())
        {
            ball.paddlenudge(1, -Lpaddle.get_speed());

        }
        else if (IsKeyDown(KEY_S) && ball.get_speedy() + Lpaddle.get_speed() / 2 < ball.get_rawspeed() - ball.get_rawspeed() / 5 && Lpaddle.get_arzaidejas())
        {
            ball.paddlenudge(1, Lpaddle.get_speed());
        }
    }

    if ((ball.get_y() + ball.get_radius() >= GetScreenHeight() && ball.get_speedy() > 0) || (ball.get_y() - ball.get_radius() <= 0 && ball.get_speedy() < 0)) //ball with bottom and upper walls
    {
        ball -= -1;
    }

    if (ball.get_x() + ball.get_radius() >= GetScreenWidth()) //scoring
    {
        leftscore++;
        pradeda = 1;
        reset(ball, laser, Lpaddle, Rpaddle, bricks, shooting, pradeda);
    }
    else if (ball.get_x() - ball.get_radius() <= 0)
    {
        rightscore++;
        pradeda = 0;
        reset(ball, laser, Lpaddle, Rpaddle, bricks, shooting, pradeda);
    }

    for (int i = 0; i < 21; i++) //ball with bricks
    {
        if (CheckCollisionCircleRec(Vector2{ ball.get_x(), ball.get_y() }, ball.get_radius(), Rectangle{ bricks.get_brickx(i), bricks.get_bricky(i), bricks.get_brickwidth(), bricks.get_brickheight() }))
        {
            //checking corners
            if (CheckCollisionPointCircle(Vector2{ bricks.get_brickx(i), bricks.get_bricky(i)}, Vector2{ball.get_x(), ball.get_y()}, ball.get_radius())) //upper left corner
            {
                if (ball.get_speedx() > 0 && ball.get_speedy() > 0) //jeigu kaktomusa su kampu
                {
                    ~ball;
                    ball += -1;
                    ball -= -1;
                    ball.edgenudge(-1, -1);
                }
                else if (ball.get_speedx() > 0) //jeigu atsimusa i kairine siena
                {
                    ball += -1;
                }
                else //jeigu atsimusa i virsutine siena
                {
                    ball -= -1;
                }
            }
            else if (CheckCollisionPointCircle(Vector2{ bricks.get_brickx(i) + bricks.get_brickwidth(), bricks.get_bricky(i)}, Vector2{ball.get_x(), ball.get_y()}, ball.get_radius())) // upper right corner
            {
                if (ball.get_speedx() < 0 && ball.get_speedy() > 0) //jeigu kaktomusa su kampu
                {
                    ~ball;
                    ball.edgenudge(1, -1);
                }
                else if (ball.get_speedx() < 0) //jeigu atsimusa i desinine siena
                {
                    ball += -1;
                }
                else //jeigu atsimusa i virsutine siena
                {
                    ball -= -1;
                }
            }
            else if (CheckCollisionPointCircle(Vector2{ bricks.get_brickx(i) + bricks.get_brickwidth(), bricks.get_bricky(i) + bricks.get_brickheight()}, Vector2{ball.get_x(), ball.get_y()}, ball.get_radius())) //down right corner
            {
                if (ball.get_speedx() < 0 && ball.get_speedy() < 0) //jeigu kaktomusa su kampu
                {
                    ~ball;
                    ball += -1;
                    ball -= -1;
                    ball.edgenudge(1, 1);
                }
                else if (ball.get_speedx() < 0) //jeigu atsimusa i desinine siena
                {
                    ball += -1;
                }
                else //jeigu atsimusa i apatine siena
                {
                    ball -= -1;
                }
            }
            else if (CheckCollisionPointCircle(Vector2{ bricks.get_brickx(i), bricks.get_bricky(i) + bricks.get_brickheight() }, Vector2{ball.get_x(), ball.get_y()}, ball.get_radius())) //down left corner
            {
                if (ball.get_speedx() > 0 && ball.get_speedy() < 0) //jeigu kaktomusa su kampu
                {
                    ~ball;
                    ball.edgenudge(-1, 1);
                }
                else if (ball.get_speedx() > 0) //jeigu atsimusa i kairine siena
                {
                    ball += -1;
                }
                else //jeigu atsimusa i apatine siena
                {
                    ball -= -1;
                }
            }//jeigu nekabino kampu tai tiesiog atsimusa i siena
            else if ((CheckCollisionCircleRec(Vector2{ ball.get_x(), ball.get_y()}, ball.get_radius(), Rectangle{bricks.get_brickx(i), bricks.get_bricky(i), 1, bricks.get_brickheight()}) && ball.get_speedx() > 0) || (CheckCollisionCircleRec(Vector2{ball.get_x(), ball.get_y()}, ball.get_radius(), Rectangle{bricks.get_brickx(i) + bricks.get_brickwidth(), bricks.get_bricky(i), 1, bricks.get_brickheight()}) && ball.get_speedx() < 0)) //left or right wall
            {
                ball += -1;
            }
            else if ((CheckCollisionCircleRec(Vector2{ ball.get_x(), ball.get_y()}, ball.get_radius(), Rectangle{bricks.get_brickx(i), bricks.get_bricky(i), bricks.get_brickwidth(), 1}) && ball.get_speedy() > 0) || (CheckCollisionCircleRec(Vector2{ball.get_x(), ball.get_y()}, ball.get_radius(), Rectangle{bricks.get_brickx(i), bricks.get_bricky(i) + bricks.get_brickheight(), bricks.get_brickwidth(), 1}) && ball.get_speedy() < 0)) //upper or down wall
            {
                ball -= -1;
            }
            
            ball += 1.05; //5% padidinu greiti
            ball -= 1.05;
            bricks - i; //plytos naikinimas
            i = 100;
        }
    }
}


int main() {
    srand(time(0));
    cout << "starting" << endl;
    const int screenwidth = 1400;
    const int screenheight = 884; //884 yra default
    InitWindow(screenwidth, screenheight, "Brick Pong");
    SetTargetFPS(144);

    Menu menu(float(screenwidth) / 2, float(screenheight) / 2, 50);
    Ball ball(18, 4);//radius, initial speed
    Bricks bricks(38, 68);//width, height
    Laser laser;

    int pradeda = rand() % 2;  //0 - pradeda kaire, 1 - pradeda desine
    bool shooting = true; //kamuoliukas yra taikomas
    bool arp1 = true, arp2 = true;

    while (!IsKeyDown(KEY_SPACE) && !WindowShouldClose())       //menu
    {
        BeginDrawing();
        menu.update(arp1, arp2);
        menu.draw();
        EndDrawing();
    }
    Color Lcolor = BLUE, Rcolor = BLUE;
    if (arp1)
        Lcolor = GREEN;
    if (arp2)
        Rcolor = GREEN;
    LeftPaddle Lpaddle(10, 25, 120, 3.5, arp1); //x, width, height, speed
    RightPaddle Rpaddle(screenwidth - 25 - 10, 25, 120, 3.5, arp2);//x(screenwidth - width - 10), width, height, speed
    reset(ball, laser, Lpaddle, Rpaddle, bricks, shooting, pradeda);
    while(!WindowShouldClose())  //game loop
    {
        BeginDrawing();

        //updating
        if (shooting == false)
        {
            ball.update();
            Rpaddle.update(ball.get_y());
            Lpaddle.update(ball.get_y());
        }
        else
        {
            laser.Update(pradeda, Lpaddle.get_arzaidejas(), Rpaddle.get_arzaidejas(), shooting);
            if (shooting == false)
            {
                ball.updatespeed(laser.direction(true), laser.direction(false));
            }
        }

        //checking for collisions
        checkcollisions(ball, Rpaddle, laser, Lpaddle, bricks, shooting, pradeda);

        //drawing
        ClearBackground(Green);
        DrawCircle(screenwidth/2, screenheight/2, GetScreenHeight()/4, LightGreen);
        DrawLine(screenwidth / 2, 0, screenwidth / 2, screenheight, WHITE);
        DrawLine(10 + Lpaddle.get_width()/2, 0, 10 + Lpaddle.get_width() / 2, screenheight, LightGray);
        DrawLine(screenwidth - 10 - Rpaddle.get_width() / 2, 0, screenwidth - 10 - Rpaddle.get_width() / 2, screenheight, LightGray);
        DrawText(TextFormat("%i", leftscore), screenwidth / 4 - 20, 20, 80, Lcolor);
        DrawText(TextFormat("%i", rightscore), 3 * screenwidth / 4 - 20, 20, 80, Rcolor);

        if(shooting == true)
        laser.Draw(ball.get_x());
        ball.draw();
        Lpaddle.draw();
        Rpaddle.draw();
        bricks.draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}