#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "Curling.h"
#include <Stone.h>
#include <vector>
#include <string>

#define PI 3.14159265

using namespace std;

const int BOARD_WIDTH = 1250;
const int BOARD_HEIGHT = 165;
bool Program_on = true;
bool Menu_open = true;

Curling menu_launch()
{
    char menu_mode = 'M';
    int UI_pt_win = 1;
    sf::Time m1=sf::seconds(1.0/60.0);
    sf::Time m2;
    sf::Clock menu_clock;

    sf::RenderWindow menu(sf::VideoMode(800, 800), "Menu");

    while (menu.isOpen())
    {
        // Process events
        sf::Event event;
        while (menu.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                menu.close();
            }
        }
        menu.clear(sf::Color(0,0,255));

        sf::Font font;
        if (!font.loadFromFile("sansation.ttf"))
        {
            //return EXIT_FAILURE; (unclear what i should return here)
        }
        sf::Text menu_list[4];
        menu_list[0].setString("2 - press for 2 player game");
        menu_list[1].setString("T - press for Training Mode");
        menu_list[2].setString("O - press for Options");
        menu_list[3].setString("Q - press for Quit");

        for (int t=0; t<4; t++)
        {
            //menu_list[t].setOrigin(menu_list[t].getLocalBounds().width/2,menu_list[t].getLocalBounds().height/2);
            menu_list[t].setPosition(100,100+50*t);
            menu_list[t].setFont(font);
            menu_list[t].setCharacterSize(50);
            menu_list[t].setColor(sf::Color::White);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            menu.close();
            Menu_open = false;
            return Curling(1,UI_pt_win);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
            menu.close();
            Menu_open = false;
            return Curling(0,UI_pt_win);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
        {
            menu_mode = 'O';
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            Program_on = false;
            Menu_open = false;
            menu.close();
            // i need to return something here i think but not sure what
        }


        if (menu_mode = 'M')
        {
            for (int m=0; m<4; m++)
            {
                menu.draw(menu_list[m]);
            }
        }
        else if (menu_mode = 'O')
        {
            // put options here
        }

        menu.display();
        sf::sleep(m1);
        m2=menu_clock.getElapsedTime();
    }
}


int main()
{
    Curling game = menu_launch();

    while (Program_on)
    {
        if (Menu_open)
        {
            Curling game = menu_launch();
        }
        // Create Curling game object based on user inputs
        //Curling game(1,4);
        int NUM_OF_STONES;
        if(game.getPlayType() == 0)
            NUM_OF_STONES = 8;
        else
            NUM_OF_STONES = 16;
        Stone s_b[NUM_OF_STONES];

        // Initialize stone position and coloring
        game.prepareStones(s_b);

        sf::Time t1=sf::seconds(1.0/60.0);
        sf::Time t2;
        sf::Time t_sweep[2];
        sf::Clock myclock;


        // Change below constants to update minimum spin
        const float DEGREE_PER_TURN = 10;
        const float TIME_PER_TURN = 10;
        const float MIN_SPIN = DEGREE_PER_TURN * (PI / 180) / 8 / 60;

        bool Stone_inPlay = false;
        int Stone_turn = 0;

        char Play_Mode = 'B';
        char last_Mode = 'B';
        bool Changed_Mode = false;
        bool lastThrow = true;
        int setNum = 0;



        sf::RenderWindow app(sf::VideoMode(1400, 600), "SFML window");

        /*const sf::Vector2f screenCenter(700,300);
        const sf::Vector2f screenSize(1400,600);
        sf::View view1(screenCenter,screenSize);
        view1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
        app.setView(view1);

        const sf::Vector2f buttonLocation(180,BOARD_HEIGHT / 2);
        const sf::Vector2f viewSize(120,120);
        sf::View view2(buttonLocation,viewSize);
        view2.setViewport(sf::FloatRect(0.4f, 0.4f, .3f, .3f));
        app.setView(view2);*/

        // Create window displaying close up of House
        sf::RenderWindow houseZoom(sf::VideoMode(350, 350), "House Zoom");
        game.createHouseView(houseZoom,app);




        // Upload font
        sf::Font font;
        if (!font.loadFromFile("sansation.ttf"))
        {
            return EXIT_FAILURE;
        }

        // Create House, boundary lines, initial placement circles, and label identifying play type (practice or one-on-one)
        sf::CircleShape Targets[4];
        sf::RectangleShape lines[8];
        sf::CircleShape resting_Spots[NUM_OF_STONES];
        sf::Text gameTypeLabel;
        game.drawRink(Targets,lines, resting_Spots,s_b,NUM_OF_STONES,gameTypeLabel,font);

        // Draw zoom of House
        sf::CircleShape houseTargets[4];
        sf::RectangleShape houseLines[2];
        game.drawHouseZoom(houseTargets,houseLines);

        // Invalid Throw Message
        sf::Text message("Invalid Throw",font,15);
        message.setPosition(1000,145);
        message.setColor(sf::Color::Red);

        //Sweeping Picture
        sf::Texture sweeping_pic;
        if (!sweeping_pic.loadFromFile("sweeping.png"))
        {
            return EXIT_FAILURE;
        }
        sf::Sprite sweeping(sweeping_pic);
        sweeping.scale(0.2,0.2);
        float sweep_w = sweeping.getTextureRect().width;
        float sweep_h = sweeping.getTextureRect().height;
        sweeping.setOrigin(sweep_w,(1/3)*sweep_h);
        sweeping.setPosition(1000,130);
        float ms = 1;
        float pos_sw_x = 0.0;
        float pos_sw_y = 0.0;


        //launch arrow
        sf::Texture arrow_pic;
        if (!arrow_pic.loadFromFile("arrow.png"))
        {
            return EXIT_FAILURE;
        }
        sf::Sprite arrow(arrow_pic);
        arrow.scale(0.1,0.1);
        float arrow_w = arrow.getTextureRect().width;
        float arrow_h = arrow.getTextureRect().height;
        arrow.setOrigin(arrow_w,arrow_h/2-5);
        arrow.setPosition(1110,BOARD_HEIGHT/2);


        // Launch rotation icons
        sf::Texture clockwiseTexture;
        if (!clockwiseTexture.loadFromFile("clockwise_v2.png"))
        {
            return EXIT_FAILURE;
        }
        sf::Sprite clockwiseArrow(clockwiseTexture);
        clockwiseArrow.scale(0.2,0.2);
        float clockwise_w = clockwiseArrow.getTextureRect().width;
        float clockwise_h = clockwiseArrow.getTextureRect().height;
        clockwiseArrow.setOrigin(clockwise_w / 2,clockwise_h / 2);
        clockwiseArrow.setPosition(1325 - clockwise_w / 2,BOARD_HEIGHT / 2);

        sf::Texture counterClockwiseTexture;
        if (!counterClockwiseTexture.loadFromFile("counterclockwise_v2.png"))
        {
            return EXIT_FAILURE;
        }
        sf::Sprite counterClockwiseArrow(counterClockwiseTexture);
        counterClockwiseArrow.scale(0.1,0.1);
        float counterClockwise_w = counterClockwiseArrow.getTextureRect().width;
        float counterClockwise_h = counterClockwiseArrow.getTextureRect().height;
        counterClockwiseArrow.setOrigin(counterClockwise_w / 2,counterClockwise_h / 2);
        counterClockwiseArrow.setPosition(1393 - counterClockwise_w / 2,BOARD_HEIGHT / 2);

        int spinCounter = 0;

        //Create the Score Board
        sf::RectangleShape sb[6];
        sf::Text sb_Text[6];
        sf::Vector2f sb_size(100,50);
        //game.drawScoreboard(sb,sb_Text,sb_size,font);

        sb[0].setFillColor(sf::Color::Green);
        sb[1].setFillColor(sf::Color::Black);
        sb[2].setFillColor(sf::Color::Yellow);
        sb_Text[0].setString("Score");
        sb_Text[1].setString("Set");
        sb_Text[2].setString("Score");

        for (int b=0; b<6; b++)
        {
            if (b<3)
            {
                sb[b].setSize(sb_size);
                sb[b].setOrigin(sb_size.x/2,sb_size.y/2);
                sb[b].setOutlineColor(sf::Color::Black);
                sb[b].setOutlineThickness(-2.0);
                sb[b].setPosition(BOARD_WIDTH/2+(b-1)*sb_size.x,195);
                sb_Text[b].setFont(font);
                sb_Text[b].setOrigin(sb_Text[b].getLocalBounds().width/2,sb_Text[b].getLocalBounds().height/2);
                sb_Text[b].setCharacterSize(30);
                sb_Text[b].setPosition(sb[b].getPosition());
                sb_Text[b].setColor(sf::Color::Black);
            }
            else
            {
                sb[b].setSize(sb_size);
                sb[b].setOrigin(sb_size.x/2,sb_size.y/2);
                sb[b].setOutlineColor(sf::Color::Black);
                sb[b].setOutlineThickness(-2.0);
                sb[b].setPosition(BOARD_WIDTH/2+(b-4)*sb_size.x,245);
                sb_Text[b].setFont(font);
                sb_Text[b].setOrigin(sb_Text[b].getLocalBounds().width/2,sb_Text[b].getCharacterSize()/2);
                sb_Text[b].setCharacterSize(20);
                sb_Text[b].setPosition(sb[b].getPosition());
                sb_Text[b].setString("0");
                sb_Text[b].setColor(sf::Color::Black);
            }
        }
        sb_Text[1].setColor(sf::Color::White);

        sf::Vector2u window_size;
        window_size.x = BOARD_WIDTH;
        window_size.y = BOARD_HEIGHT;

        // Initialize Scoreboard Data
        int winnerDeclaredCounter = 0;

        // Start the game loop
        while (app.isOpen())
        {
            // Process events
            sf::Event event;
            while (app.pollEvent(event))
            {
                // Close window : exit
                if (event.type == sf::Event::Closed)
                {
                    Menu_open = true;
                    app.close();
                    houseZoom.close();
                }
            }

            // Clear screen
            if (Play_Mode != 'P')
            {
                app.clear(sf::Color(255,255,255));
                houseZoom.clear(sf::Color::White);
            }

            // Draw the board
            Stone_inPlay = game.checkPlay_Status(s_b, NUM_OF_STONES);

            if (!Stone_inPlay)
            {
                // Draw array with length and direction as specified by user
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (arrow.getRotation() > 315 || arrow.getRotation() <= 45))
                {
                    arrow.rotate(-.25);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (arrow.getRotation() >= 315 || arrow.getRotation() < 45))
                {
                    arrow.rotate(.25);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && arrow.getScale().x < .2)
                {
                    arrow.scale(1.01,1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && arrow.getScale().x > .05)
                {
                    arrow.scale(0.99,1.0);
                }
                app.draw(arrow);

                // Draw and rotate rotation icon per magnitude of spin chosen by user
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
                {
                    if(spinCounter <= 3)
                    {
                        spinCounter++;
                        clockwiseArrow.setPosition(clockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 12);
                        counterClockwiseArrow.setPosition(counterClockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 10);
                    }
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                {
                    if(spinCounter >= -3)
                    {
                        spinCounter--;
                        counterClockwiseArrow.setPosition(counterClockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 12);
                        clockwiseArrow.setPosition(clockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 10);
                    }
                }

                if(spinCounter >= 0)
                {
                    app.draw(clockwiseArrow);
                    clockwiseArrow.rotate(spinCounter * 5);
                }
                else
                {
                    app.draw(counterClockwiseArrow);
                    counterClockwiseArrow.rotate(spinCounter * 5);
                }
            }

            // Draw House
            for (int t=0; t<4; t++)
            {
                app.draw(Targets[t]);
                app.draw(houseTargets[t]);
                houseZoom.draw(Targets[t]);
            }

            // Draw boundary lines
            for (int l=0; l<8; l++)
            {
                app.draw(lines[l]);
                houseZoom.draw(lines[l]);
            }

            for(int i = 0; i < 2; i++)
            {
                app.draw(houseLines[i]);
            }

            //Draw Score Board
            for (int b=0; b<6; b++)
            {
                app.draw(sb[b]);
                app.draw(sb_Text[b]);
            }
            // Draw stone array
            for (int s=0; s<NUM_OF_STONES; s++)
            {
                app.draw(resting_Spots[s]);
                app.draw(s_b[s]);
                houseZoom.draw(s_b[s]);
            }

            // Draw play type label
            app.draw(gameTypeLabel);

            // game mode detection
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
                Play_Mode = 'P';
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                app.close();
            }

            // Deliver stone with speed, direction, and spin as specified by user and check for collisions
            if (Play_Mode !='P')
            {
                // Check for invalid throws
                if (!lastThrow)
                {
                    app.draw(message);
                }
                if (!game.checkPlay_Status(s_b,NUM_OF_STONES))
                {
                    if (game.inValid_Throw(s_b[Stone_turn - 1]))
                    {
                        lastThrow = false;
                        s_b[Stone_turn - 1].setPosition(-5,-5);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !game.checkPlay_Status(s_b,NUM_OF_STONES))
                {
                    lastThrow = true;
                    if (Stone_turn < NUM_OF_STONES)
                    {
                        // Begin turn by changing status to "in play" and setting the initial speed, direction, and speed to values specified by user
                        s_b[Stone_turn].changeStatus();
                        s_b[Stone_turn].setPosition(1110,BOARD_HEIGHT/2);
                        s_b[Stone_turn].setInitialSpeed(arrow.getScale().x*45);
                        s_b[Stone_turn].setSpin(spinCounter * MIN_SPIN);


                        if(arrow.getRotation() <= 45)
                        {
                            s_b[Stone_turn].setInitialDirection((180 + arrow.getRotation())*PI/180);
                        }
                        else
                        {
                            s_b[Stone_turn].setInitialDirection((180 - (360 - arrow.getRotation()))*PI/180);
                        }
                        Stone_turn++;

                        // Return spin icons to default settings
                        spinCounter = 0;
                        clockwiseArrow.setPosition(1325 - clockwise_w / 2,BOARD_HEIGHT/2);
                        counterClockwiseArrow.setPosition(1393 - counterClockwise_w / 2,BOARD_HEIGHT/2);

                        // Return arrow to normal length and rotation
                        arrow.setScale(0.1,0.1);
                        arrow.setRotation(0);
                    }
                }

                Changed_Mode =false;
                for (int b=0; b<NUM_OF_STONES; b++)
                {
                    // Setting Friction
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && game.checkPlay_Status(s_b,NUM_OF_STONES))
                    {
                        int x = game.getClostest_movingStone(s_b,sf::Mouse::getPosition(),NUM_OF_STONES);
                        s_b[x].setFriction(0.8*5*9.81*.0168/60);
                        pos_sw_x = s_b[x].getPosition().x;
                        pos_sw_y = s_b[x].getPosition().y;

                        app.draw(sweeping);
                        houseZoom.draw(sweeping);
                        if (t_sweep[0] == sf::seconds(0.0))
                        {
                            t_sweep[0] = myclock.getElapsedTime();
                        }
                        else
                        {
                            t_sweep[1] = myclock.getElapsedTime();
                        }

                        if(t_sweep[1]-t_sweep[0] > sf::seconds(0.15))
                        {
                            t_sweep[0] = t_sweep[1];
                            ms *= -1;
                        }
                        /*for (int s=0; s<Stone_turn; s++)
                        {
                            if (s_b[s].getSpeed() != 0)
                            {
                                pos_sw_x = s_b[s].getPosition().x;
                                pos_sw_y = s_b[s].getPosition().y;
                            }
                        }*/
                        sweeping.setPosition(pos_sw_x-8.0,pos_sw_y-18.0+5.0*ms);
                    }
                    else
                    {
                        t_sweep[0]=sf::seconds(0.0);
                        t_sweep[1]=sf::seconds(0.0);
                        sweeping.setPosition(0.0,0.0);
                        s_b[b].setFriction();
                    }

                    // Update speed, direction, velocity and then move Stone
                    s_b[b].setSpeed();
                    s_b[b].setDirection();
                    s_b[b].setVelocity();
                    s_b[b].makeMove();
                }

                /*for(int i = 0; i < 16; i++)
                {
                    cout << "Stone Number: " << i << " ";
                    cout << "Postion: " << s_b[i].getPosition().x << " " << s_b[i].getPosition().y << endl;
                }*/

                // Check for stone collisions
                int collisionStones[2];
                while(Stone::isCollision(s_b,NUM_OF_STONES,collisionStones, window_size))
                {
                    //cout << "Collison!" << endl;
                    //cout << collisionStones[0] << " " << collisionStones[1] << endl;

                    // Find and set new velocities post collision for stones that collided
                    sf::Vector2f newStone1Velocity = s_b[collisionStones[0]].findVelocityPostCollision(s_b[collisionStones[1]]);
                    sf::Vector2f newStone2Velocity = s_b[collisionStones[1]].findVelocityPostCollision(s_b[collisionStones[0]]);

                    // Update velocities for colliding stones
                    s_b[collisionStones[0]].updatePostCollision(newStone1Velocity);
                    s_b[collisionStones[1]].updatePostCollision(newStone2Velocity);

                    // Move stones with new post_collision velocities while collision continues
                    while(Stone::isCollision(s_b[collisionStones[0]],s_b[collisionStones[1]]))
                    {
                        s_b[collisionStones[0]].makeMove();
                        s_b[collisionStones[1]].makeMove();
                    }
                }

                // Check for Wall Collisions
                int stoneNumber;
                while(Stone::isWallCollision(s_b,NUM_OF_STONES,stoneNumber, window_size))
                {
                    s_b[stoneNumber].setFillColor(sf::Color::White);
                    s_b[stoneNumber].setOutlineColor(sf::Color::White);
                    s_b[stoneNumber].setInitialSpeed(0);
                    s_b[stoneNumber].changeStatus();
                    s_b[stoneNumber].setPosition(-5,-5);
                }

                // Display stone in House zoom if applicable
                vector<Stone> houseStones;
                game.updateHouseZoom(s_b,houseStones,NUM_OF_STONES,Targets[0]);
                for(int i = 0; i < houseStones.size(); i++)
                    app.draw(houseStones[i]);

                // Add marker above closest stone if in House
                if(!game.checkPlay_Status(s_b,NUM_OF_STONES))
                {
                    int closest = game.findClosestStone(s_b,NUM_OF_STONES);
                    if(closest != -1 && game.inHouse(s_b[closest], Targets[0]))
                    {
                        sf::CircleShape triangle(5,3);
                        game.markClosestStone(triangle,closest,s_b);

                        app.draw(triangle);
                        houseZoom.draw(triangle);
                    }

                    // Add marker to closest stone in House view
                    //int closestInHouse = game.findClosestStone(houseStones);
                    /*if(closestInHouse != -1)
                    {
                        sf::CircleShape triangle(5 * 2.5,3);

                    }*/
                }

                // Determine winner of end and number of points earned if finished end
                if(game.getPlayType() == 1 && Stone_turn == NUM_OF_STONES && !game.checkPlay_Status(s_b,NUM_OF_STONES) && winnerDeclaredCounter == 0)
                {
                    winnerDeclaredCounter++;
                    // Determine winner
                    int winner = game.findClosestStone(s_b,NUM_OF_STONES);

                    // Determine number of points won if not a tie game
                    int points;
                    if(winner != -1 && game.inHouse(s_b[winner], Targets[0]))
                        points = game.findPointsScored(winner,s_b,Targets[0]);

                    // Update scoreboard if no tie
                    if(winner == -1 || !game.inHouse(s_b[winner], Targets[0]))
                        cout << "Tie game! No points earned." << endl;
                    else
                        game.updateScoreboard(winner,points,sb_Text[3],sb_Text[5]);

                    // Print current score
                    cout << "Current Score - " << "Team A: " << game.getTeam_A_Points() << " " << "Team B: " << game.getTeam_B_Points() << endl;
                }

                // Begin new end if neither player has reached required number of points
                if(Stone_turn == NUM_OF_STONES && !game.checkPlay_Status(s_b,NUM_OF_STONES) && game.getTeam_A_Points() < game.getPointsToWin() && game.getTeam_B_Points() < game.getPointsToWin())
                {
                    //Stone::resetNumberofStones();
                    Stone newStoneSet[NUM_OF_STONES];
                    for(int i = 0; i < NUM_OF_STONES; i++)
                        s_b[i] = newStoneSet[i];
                    game.prepareStones(s_b);
                    Stone_turn = 0;
                    winnerDeclaredCounter = 0;
                }

            // Pause Game
            else if (Play_Mode == 'P')
            {
                while (Play_Mode =='P')
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
                    {
                        Play_Mode ='B';
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                    {
                        Play_Mode = 'Q';
                    }
                }
            }

            }
            // Update the window
            app.display();
            houseZoom.display();
            sf::sleep(t1);
            t2=myclock.getElapsedTime();
            // std::cout << t2.asSeconds() << std::endl;

        }
    }

    return EXIT_SUCCESS;
}
