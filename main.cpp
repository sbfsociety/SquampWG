#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <iomanip>
bool ground = true;
using namespace std;
bool yes = false;

int frames = 0;

float dt;

int frameCounter = 0;
int direction = 1;

int gamestate = 0;

float val = 0;
bool release = false;

struct Vec2{
    float x, y;
};

void text(sf::Font& font, sf::RenderWindow& window, int textSize, string text, float x, float y);


class Player{
private:
    bool alive = true;
    bool onGround = false;
    bool jump = false;
    int state = 0; // 0 - no, 1 - Left, 2 - Right

    float add;

    Vec2 save;

    Vec2 vel = {0.f, 0.f};
    Vec2 pos;
    Vec2 gravity = {0.f, 2.f};
    bool space = false;
public:
    Player(Vec2 _p) : pos(_p), save(_p){};

    Vec2 playerSize = {20.f, 30.f};

    Vec2 old = {pos.x, pos.y};

    Vec2 newPos = {pos.x, pos.y};

    void Move(){
        space = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space));

        state = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){state = 1; pos.x -= 5.f;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){state = 2; pos.x += 5.f;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround){if (add < 100) add+=0.5; space = true;}
        else if (space && !onGround) {
            //cout << "Space held but NOT grounded! pos.y: " << pos.y << endl;
        }


    }

    void Update(){
        if (release && ::yes) {
            old.y-=1;
            pos.y-=20;
            // cout << "JUMPING! onGround: " << onGround << endl;
            onGround = false;
            if (-0.8 * ::val - 10 > -30.f) {
                vel.y = -0.8 * ::val - 10;
            }
            else {
                vel.y = -30.f;
            }
            // cout << vel.y << endl;


            add = 0;
            ::val = 0;
            release = false;
            ::yes = false;

        }
        old = {pos.x, pos.y};
        // onGround = false;

        Move();

        if (onGround){vel.y = 0;}

        if (!onGround) {
            vel.y += gravity.y;
            pos.y += vel.y;
        }

        //if (pos.y > 800.f) { pos = {save.x, save.y}; vel = {0.f, 0.f}; }


    }

    void Draw(sf::RenderWindow& window, sf::Font& font){
        float small = 10;
        sf::ConvexShape player;
        player.setPointCount(4);
        double value;
        if (-0.8 * add - 10 > -30.f) {
            value = -0.8 * add - 10;
        }
        else {
            value = -30.f;
        }

        ::frames++;

        value = (round(value/5))*5;


        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << value * -1;
        std::string result = ss.str();

        text(font, window,  20, result, pos.x, pos.y);
        if (space) {
            player.setPoint(0, sf::Vector2f(pos.x - playerSize.x, pos.y - playerSize.y + small));
            player.setPoint(1, sf::Vector2f(pos.x, pos.y - playerSize.y + small));
            text(font, window,  20, result, pos.x, pos.y);
            //cout << "Mario\n";
        }
        else{


            player.setPoint(0, sf::Vector2f(pos.x - playerSize.x, pos.y - playerSize.y));
            player.setPoint(1, sf::Vector2f(pos.x, pos.y - playerSize.y));
        }
        player.setPoint(2, sf::Vector2f(pos.x, pos.y));
        player.setPoint(3, sf::Vector2f(pos.x - playerSize.x, pos.y));
        player.setFillColor(sf::Color{127,127, 127});
        player.setOutlineColor(sf::Color::Black);
        player.setOutlineThickness(2.f);

        //sf::Texture front, back, left, right, jump, fall;
        //if (!front.loadFromFile("assets/front.jpg")){}
        //if (!back.loadFromFile("assets/back.jpg")){}
        //if (!left.loadFromFile("assets/left.jpg")){}
        //if (!right.loadFromFile("assets/right.jpg")){}
        //if (!fall.loadFromFile("assets/fall.jpg")){}
        //if (!jump.loadFromFile("assets/jump.jpg")){}


        //front.setSmooth(false);
        //left.setSmooth(false);
        //right.setSmooth(false);
        //fall.setSmooth(false);
        //jump.setSmooth(false);
        //back.setSmooth(false);


        //sf::Sprite igrac(front);

        //igrac.setPosition({pos.x - 6 - small*2, pos.y - 10- small*4});

        //igrac.setScale({0.3f, 0.3f});

        //if (vel.x > 0){igrac.setTexture(right);}
        //if (vel.x < 0){igrac.setTexture(left);}
        //if (vel.y < 0){igrac.setTexture(jump);}
        //if (vel.y > 0){igrac.setTexture(fall);}

        window.draw(player);
        //window.draw(igrac);
    }


    float getAdd() const {return add;}

    Vec2 speedy() const {return vel;}
    void speedu(Vec2 _p){vel = {_p.x, _p.y};}

    bool Space() const {return space;}

    bool Ground() const {return onGround;}

    void Grounded(bool a){onGround = a;}

    int State() const {return state;}

    Vec2 get() const {return pos;}

    void set(Vec2 _p){pos = {_p.x, _p.y};}

    Vec2 saved() const {return save;}
};

void Rect(sf::RenderWindow& window, Vec2 center, float height, float width, Player& player, Vec2 finish);

class Line {
public:
    Vec2 helper;
    Vec2 begin, end;
    bool vertical = false;
    bool horizontal = false;
    bool diagonal = false;
    bool col = false;
    bool flag = false;
    bool draw = true;
    bool fill;

    Line(Vec2 _b, Vec2 _e) : begin(_b), end(_e) {
        if (_b.x == _e.x){vertical = true; /*cout << "Vertical";*/}
        else if (_b.y == _e.y){horizontal = true; /*cout <<"Horizontal";*/}
        else {diagonal = true; /*cout << "Diagonal";*/}
    }

    Line(Vec2 _b, Vec2 _e, bool _draw) : begin(_b), end(_e), draw(_draw) {
        if (_b.x == _e.x){vertical = true; /*cout << "Vertical";*/}
        else if (_b.y == _e.y){horizontal = true; /*cout <<"Horizontal";*/}
        else {diagonal = true; /*cout << "Diagonal";*/}
    }

    Line(Vec2 _b, Vec2 _e, bool _draw, bool bottom) : begin(_b), end(_e), draw(_draw), fill(bottom) {
        if (_b.x == _e.x){vertical = true; /*cout << "Vertical";*/}
        else if (_b.y == _e.y){horizontal = true; /*cout <<"Horizontal";*/}
        else {diagonal = true; /*cout << "Diagonal";*/}
    }

    void Collide(Player& player) {
        flag = false;
        col = false;
        if (horizontal) {
            // you bong bong your head here
            if ((player.get().x >= begin.x - player.playerSize.x*0.24 + 4 && player.get().x <= end.x + player.playerSize.x*1.275)) {
                if (player.old.y > end.y + player.playerSize.y && player.get().y <= begin.y + 3 + player.playerSize.y) {
                    player.set({player.get().x, begin.y + player.playerSize.y + 4});
                    flag = true;
                    player.Grounded(false);
                }

            }

            if(player.get().x > begin.x - player.playerSize.x*0.24 && player.get().x <= end.x + player.playerSize.x*1.275){
                if(abs(player.get().y - begin.y) <= 5 && player.Ground()){player.set({player.get().x, begin.y  + 4});}

            }

            if ((player.old.y <= begin.y && player.get().y >= begin.y && !flag) &&
                (player.get().x >= begin.x - player.playerSize.x*0.24 && player.get().x <= end.x + player.playerSize.x*1.275)) {
                player.Grounded(true);
                player.set({player.get().x, begin.y - 4});
                col = true;
                }

        }
        if (abs(player.get().y - begin.y) <= 5 &&
                (player.get().x >= begin.x - player.playerSize.x*0.24 && player.get().x <= end.x + player.playerSize.x*1.275)) {
            player.Grounded(true);
            // cout << "FIX\n";
            col = true;
                }
        if (vertical) {
            if ((player.get().y >= end.y && player.get().y <= begin.y) ||
            (player.old.y >= end.y && player.old.y <= begin.y)) {
                if (player.old.x < begin.x && player.get().x >= begin.x) {
                    player.set({begin.x - 3, player.get().y});
                }
                else if (player.old.x > begin.x + player.playerSize.x + 3 && player.get().x <= begin.x + player.playerSize.x + 3) {
                    player.set({begin.x + 8 + player.playerSize.x, player.get().y});
                }
            }
        }







    };
    void Draw(sf::RenderWindow& window) {
        sf::ConvexShape line;
        float width = 4;
        line.setPointCount(4);
        if (horizontal){
            line.setPoint(0, {begin.x, begin.y + width});
            line.setPoint(1, {end.x, begin.y + width});
            line.setPoint(2, {end.x, end.y});
            line.setPoint(3, {begin.x, end.y});
            if (fill) {
                line.setPoint(0, {begin.x, begin.y + 800});
                line.setPoint(1, {end.x, begin.y + 800});
                line.setPoint(2, {end.x, end.y});
                line.setPoint(3, {begin.x, end.y});
            }
        }
        if (vertical) {
            line.setPoint(0, {begin.x, begin.y});
            line.setPoint(1, {begin.x + width, begin.y});
            line.setPoint(2, {end.x + width, end.y});
            line.setPoint(3, {end.x, end.y});
        }


        line.setFillColor(sf::Color({21, 6, 1}));
        line.setOutlineColor(sf::Color{21, 6, 1 });
        line.setOutlineThickness(2.f);
        if (draw)
            window.draw(line);
    }
};

void Lines(sf::RenderWindow& window, vector<Line>& lines, Player& player) {
        int colCount = 0;
        for (auto& line : lines) {
            line.Collide(player);
            if (line.horizontal && line.col){colCount++;}
            line.Draw(window);
        }
        if (colCount == 0) {player.Grounded(false);}
    }
void text(sf::Font& font, sf::RenderWindow& window, int textSize, string text, float x, float y) {
    sf::Text st(font);
    st.setString(text);
    st.setFillColor(sf::Color::White);
    st.setCharacterSize(textSize);
    st.setPosition({x, y});
    window.draw(st);
}
void Rect(sf::RenderWindow& window, Vec2 center, float height, float width, Player& player, Vec2 finish) {
    sf::ConvexShape rect;
    rect.setPointCount(60); // More points for more oval but overral system is slower then
    sf::Clock chargeClock;
    float chargeTimer = 0.f;

    float rX = width;   // horizontal radius
    float rY = height * 1.3f;  // vertical radius

    for (int i = 0; i < 60; i++) {
        float angle = i * 2 * 3.14159f / 60; // Divide circle into 16 segments
        rect.setPoint(i, {

            center.x + rX * cos(angle),
            center.y + rY * sin(angle)
        });
    }
    /*rect.setPointCount(4);
    rect.setPoint(0, {center.x - width, center.y + height});
    rect.setPoint(1, {center.x + width, center.y + height});
    rect.setPoint(2, {center.x + width, center.y - height});
    rect.setPoint(3, {center.x - width, center.y - height});*/
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    if (abs(player.get().x - center.x) < width*1.31 && abs(player.get().y - center.y) < height*1.31) {
        chargeTimer += chargeClock.restart().asSeconds();
        if (chargeTimer >= 0.5f) {
            chargeTimer = 0.f;

        }
        ::gamestate++;
        player.set({finish.x, finish.y});
    }


    window.draw(rect);
}
void Compensate(Line& line, vector<Line>& lines) {
        if (line.vertical) {
            float plWidth = 4.f; // match player width
            lines.push_back(Line({line.begin.x, line.begin.y}, {line.begin.x + plWidth, line.begin.y}, false));
            lines.push_back(Line({line.end.x, line.end.y}, {line.end.x + plWidth, line.end.y}, false));
        }

        if (line.horizontal) {
            float plWidth = 4.f;
            lines.push_back(Line( {line.begin.x, line.end.y  + plWidth},{line.begin.x, line.begin.y}, false));
            lines.push_back(Line({line.end.x, line.end.y + plWidth}, {line.end.x, line.end.y}, false));
        }
    }

void Level(vector<Line>& lines, sf::RenderWindow& window, Player& player, sf::Font& font) {
    player.Update();
    Lines(window, lines, player);
    if (player.get().y > 900.f && !player.Ground()){player.set({player.saved().x, player.saved().y});player.speedu({0.f, 0.f});}
    player.Draw(window,font);
}

void Picture(sf::RenderWindow& window, string name, float x, float y, float scale, string ext) {
    sf::Texture texture;
    texture.loadFromFile("assets/pics/" + name + ext);
    sf::Sprite sprite(texture);
    sprite.setPosition({x, y});
    sprite.setScale({scale, scale});
    window.draw(sprite);
}





int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "Jump king");
    sf::Clock clock;
    window.setFramerateLimit(60);
    Player player({150.f, 680.f});

    vector<Line> lines;

    sf::Font font;
    if (!font.openFromFile("daniel.ttf")){return 1;}

    sf::Font bold;
    if (!bold.openFromFile("danielbd.ttf")){return 1;}

    sf::Font veryBold;
    if (!veryBold.openFromFile("danielbk.ttf")){return 1;}

    vector<Vec2> startPoints;
    startPoints.push_back({100.f, 100.f}); // 0 - Meni
    startPoints.push_back({135.f,670.f}); // 1 - Nivoi
    startPoints.push_back({135.f,670.f}); // 2

    sf::Texture background;
    if (!background.loadFromFile("assets/pics/back.jpg")){return 1;}
    sf::Sprite backgroundSprite(background);
    backgroundSprite.setPosition({0.f, 0.f});


    // lines.push_back(Line({300, 700},{400, 700}));
    // lines.push_back(Line({400, 703},{500, 703}));

    // LEVELS - 1- 30

    // 1 -------------------------------------------------------------------------

    vector<Line> lines1;
    lines1.push_back(Line({100, 700}, {700, 700}));
    for (int i = 0; i < 1; i++) {
        Compensate(lines1[i], lines1);
    }

    // 2 -------------------------------------------------------------------------

    vector<Line> lines2;
    lines2.push_back(Line({100, 700}, {300, 700}));
    lines2.push_back(Line({300, 700},{300, 550}));
    lines2.push_back(Line({300, 550},{500, 550}));
    // lines2.push_back(Line({200, 500}, {200, 300}));

    for (int i = 0; i < 3; i++) {
        Compensate(lines2[i], lines2);
    }

    // 3 -------------------------------------------------------------------------



    int originalSize = lines.size();
    for (int i = 0; i < originalSize; i++) {
        Compensate(lines[i], lines);
    }


    while(window.isOpen()) {
        ::dt = clock.restart().asSeconds();

        while(auto event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>())
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && ::gamestate == 0) {
                ::gamestate = 1;
            }
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {

                }
            }
                if (::gamestate > 0){
                    if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                        if (keyReleased->code == sf::Keyboard::Key::Space && player.Ground()) {
                            if (player.getAdd() < 100){::val = player.getAdd();}
                            else{::val = player.getAdd();}


                            ::release = true;
                            ::yes = player.Ground();
                        }
                    }
                }
        }
        window.clear(sf::Color{27,27,27});

        window.draw(backgroundSprite);


        if (::gamestate == 0) {
            frameCounter += direction;
            if (frameCounter >= 20) direction = -1;
            if (frameCounter <= -20) direction = 1;
            sf::Text play(font);
            play.setString("Press space to play");
            play.setFillColor(sf::Color::White);
            play.setCharacterSize(42);
            play.setPosition({300, 500 + float(::frameCounter/2)});
            window.draw(play);
        }

        /*if (::gamestate == 1) {

            player.Update();
            Lines(window, lines1, player);
            if (player.get().y > 900.f && !player.Ground()){player.set({player.saved().x, player.saved().y});player.speedu({0.f, 0.f});}
            player.Draw(window);
        }*/

        if (::gamestate == 1) {
            Rect(window, {600, 670}, 20, 20, player,startPoints[::gamestate]);
            text(veryBold, window, 28, "Move by hitting A and D", 100, 500);
            Level(lines1, window, player, font);
        }

        if (::gamestate == 2) {
            Rect(window, {500, 520}, 20, 20, player,startPoints[::gamestate]);
            text(veryBold, window, 28, "Charge your jump with ", 100, 200);
            Picture(window, "spaces", 480, 166, 0.20, ".png");
            Level(lines2, window, player, font);
        }



        window.display();
    }

    return 0;
}
