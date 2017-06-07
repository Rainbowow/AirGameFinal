#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <list>
#include <iostream>
#include <deque>
using namespace sf;

class Entity{
    public:
        float x,y,R;
        bool life;
        std::string name;
        Sprite sprite;
        Entity(){life=1;}

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
            y+=2;
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



class player:public Entity{
    public:
    player(){name="player";}


    int deadcount;

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

            RenderWindow window(VideoMode(600,1080),"AirGameFinal");
            window.setFramerateLimit(60);
            Texture Background,Player;
            Player.loadFromFile("player.png");

            Background.loadFromFile("background.jpg");
            Sprite mBackground(Background);

            Texture mbullet;
            mbullet.loadFromFile("bullet.png");

            Texture enemy;
            enemy.loadFromFile("enemy.png");


            Music music;
            music.openFromFile("backgroundmusic.ogg");
            music.play();
            music.setLoop(true);



            std::deque<Entity*> entities;

            player *p = new player();
            p->settings(Player,260,900,1);
            entities.push_back(p);

                while(window.isOpen()){
                    Event event;
                    while(window.pollEvent(event)){
                        if(event.type==Event::Closed)
                            window.close();
                        if (event.type == Event::KeyPressed)
                        if (event.key.code == Keyboard::Space){
                            bullet *b = new bullet();
                            b->settings(mbullet,p->x+40,p->y,1);
                            entities.push_back(b);
                        }
                    }

                    float playerSpeed=10;
                    if (Keyboard::isKeyPressed(Keyboard::Down)&&p->y<=1080){p->y+=playerSpeed;}
                    if (Keyboard::isKeyPressed(Keyboard::Up)&&p->y>=100){p->y-=playerSpeed;}
                    if (Keyboard::isKeyPressed(Keyboard::Left)&&p->x>=0){p->x-=playerSpeed;}
                    if (Keyboard::isKeyPressed(Keyboard::Right)&&p->x<=500){p->x+=playerSpeed;}


                    int randomflighty=rand()%(0+500)-500;
                    int randomflightx=rand()%600;
                    enemyflight *e= new enemyflight();
                    e->settings(enemy,randomflightx,randomflighty,1);
                    entities.push_back(e);



                    for(auto a:entities)
                    for(auto b:entities){
                        if(a->name=="enemyflight"&&b->name=="bullet")
                        if(isCollide(a,b)){
                            a->life=false;
                            b->life=false;
                        }
                    }

                    for(auto i=entities.begin();i!=entities.end();){
                        Entity *e=*i;
                        e->update();
                        if(e->life==false){i=entities.erase(i);delete e;}
                        else i++;
                    }

                    window.clear();
                    window.draw(mBackground);
                    for(auto i:entities) i->draw(window);
                    window.display();
                }
        }

};


