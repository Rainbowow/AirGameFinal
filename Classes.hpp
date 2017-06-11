#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <list>
#include <iostream>
#include <deque>
#include <windows.h>
using namespace sf;
int scores=0;
float playerSpeed=10;
int EnemySpeed=2;
void musicplay(Music &t,std::string musicname,bool isLoop){

        t.openFromFile(musicname);
        t.play();
        t.setLoop(isLoop);
}

class Entity{
    public:
        float x,y,R;
        bool life;
        std::string name;
        Sprite sprite;
        Entity(){life=1;}
        int deadcount=0;
        void settings(Texture &t,int X,int Y,int radius=1){
            sprite.setTexture(t);
            x=X;
            y=Y;
            R=radius;
        }

        virtual void update(){};

        void draw(RenderWindow &window){
            sprite.setPosition(x,y);
            window.draw(sprite);
        }

        virtual ~Entity(){};
};

class enemyflight: public Entity{
    public:
        enemyflight(){name="enemyflight";}

        void update(){
            y+=EnemySpeed;
            if(y>1080){
                life=false;
            }
        }
};




class bullet:public Entity{
    public:
    bullet(){name="bullet";}

    void update(){
        y-=7;
        /////sidemanage/////
        if(y<0){
            life=false;
        }
    }
};

class ebullet:public Entity{
    public:
    ebullet(){name="ebullet";}

    void update(){
        y+=5;
        /////sidemanage/////
        if(y>1080){
            life=false;
        }
    }
};

class player:public Entity{
    public:
    player(){name="player";}



    void update(){

    }
};
bool isCollide(Entity *a,Entity *b){
    return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}

class Game{
    public:
        void run(){

            srand(time(0));


            RenderWindow window(VideoMode(800,1080),"AirGameFinal");
            window.setFramerateLimit(60);
            Texture gameover;
            gameover.loadFromFile("gameover.jpg");

            Texture eBullet;
            eBullet.loadFromFile("ebullet.png");

            Texture Background,Player;
            Player.loadFromFile("player.png");

            Background.loadFromFile("background.jpg");
            Sprite mBackground(Background);

            Texture mbullet;
            mbullet.loadFromFile("bullet.png");

            Texture enemy;
            enemy.loadFromFile("enemy.png");

            Texture explosion;
            explosion.loadFromFile("explosion.png");

            Font font;
            font.loadFromFile("pixelmix.ttf");
            Text text;
            text.setFont(font);
            text.setPosition(600,0);
            text.setString("Scores");
            Text text1;
            text1.setFont(font);
            text1.setPosition(750,0);

            Sprite Gameover(gameover);

            Music backgroundmusic;
            Music explosionsound;
            musicplay(backgroundmusic,"backgroundmusic.ogg",true);



            std::deque<Entity*> entities;

            player *p = new player();
            p->sprite.setOrigin(42,45);
            p->settings(Player,260,900,45);
            entities.push_back(p);


                while(window.isOpen()){
                    Event event;
                    while(window.pollEvent(event)){
                        if(event.type==Event::Closed)
                            window.close();
                        if (event.type == Event::KeyPressed)
                        if (event.key.code == Keyboard::Space){
                            bullet *b = new bullet();
                            b->sprite.setOrigin(15,25);
                            b->settings(mbullet,p->x+40,p->y,25);
                            entities.push_back(b);
                        }
                    }



                    if (Keyboard::isKeyPressed(Keyboard::Down)&&p->y<=900){p->y+=playerSpeed;}
                    if (Keyboard::isKeyPressed(Keyboard::Up)&&p->y>=0){p->y-=playerSpeed;}
                    if (Keyboard::isKeyPressed(Keyboard::Left)&&p->x>=0){p->x-=playerSpeed;}
                    if (Keyboard::isKeyPressed(Keyboard::Right)&&p->x<=500){p->x+=playerSpeed;}


                    int randomflighty=rand()%(0+100)-100;
                    int randomflightx=rand()%400;

                    if(clock()%100==0){
                    enemyflight *e= new enemyflight();
                    e->sprite.setOrigin(100,100);
                    e->settings(enemy,randomflightx,randomflighty,100);
                    entities.push_back(e);
                    ebullet *a = new ebullet();
                    a->settings(eBullet,e->x,e->y,10);
                    entities.push_back(a);
                    }



                    for(auto a:entities)
                    for(auto b:entities){
                        if(a->name=="enemyflight"&&b->name=="bullet")
                        if(isCollide(a,b)){
                            a->life=false;
                            b->life=false;
                            scores+=10;

                            musicplay(explosionsound,"explosionsound.ogg",false);

                            Entity *e = new Entity();
                            e->settings(explosion,a->x-50,a->y,1);
                            e->name="explosion";
                            entities.push_back(e);
                        }
                        if(a->name=="ebullet"&&b->name=="player")
                        if(isCollide(a,b)){
                            a->life=0;
                            b->deadcount+=1;
                            if(b->deadcount<2){
                                b->deadcount+=1;
                            }
                            if(b->deadcount>3){
                                goto gameover;
                            }

                        }
                        if(scores>100){
                            EnemySpeed+=2;
                        }
                    }

                    text1.setString(std::to_string(scores));

                    for(auto i=entities.begin();i!=entities.end();){
                        Entity *e=*i;
                        e->update();
                        if(e->life==false){i=entities.erase(i);delete e;}
                        else i++;
                    }

                    window.clear();
                    window.draw(mBackground);
                    for(auto i:entities){
                        i->draw(window);
                        if(i->name=="explosion"){
                            i->life=false;
                        }
                    }
                    window.draw(text);
                    window.draw(text1);
                    window.display();
                }
            gameover:
                window.draw(Gameover);
        }

};


