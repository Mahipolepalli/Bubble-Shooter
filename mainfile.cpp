#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include <cmath>
#include <sstream>

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

int move_bubbles(vector<Bubble> &bubbles, Shooter shooter, vector<Bullet> &bullets, int &score){
    // move all bubbles
    int sho_x = shooter.get_head_center_x();
    int sho_y = shooter.get_head_center_y();
    int sho_rectx = shooter.get_body_center_x();
    int sho_recty = shooter.get_body_center_y();

    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
        int bub_x = bubbles[i].get_center_x();
        int bub_y = bubbles[i].get_center_y();
        double dist = sqrt((pow((sho_x - bub_x), 2) + pow((sho_y - bub_y), 2)));
        double distrect = sqrt((pow((sho_rectx - bub_x), 2) + pow((sho_recty - bub_y), 2)));
        if(dist < bubbles[i].get_radius() + shooter.get_head_radius()) return 1;
        else if(distrect < bubbles[i].get_radius() + shooter.get_body_height()) return 1;
        for(unsigned int j=0; j<bullets.size(); j++){
            int bul_x = bullets[j].get_center_x();
            int bul_y = bullets[j].get_center_y();
            double collision = sqrt((pow((bul_x - bub_x), 2) + pow((bul_y - bub_y), 2)));
            if(collision < bubbles[i].get_radius() + sqrt(pow(bullets[j].get_width(), 2) + pow(bullets[j].get_height(), 2)))
            {
                 score++;
                 if(bubbles[i].get_radius() == 4*BUBBLE_DEFAULT_RADIUS)
                 {
                      bubbles.erase(bubbles.begin() + i);
                      bullets.erase(bullets.begin() + j);
                      bubbles.push_back(Bubble(bub_x, bub_y, 2*BUBBLE_DEFAULT_RADIUS, -2*BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(225,180,255)));
                      bubbles.push_back(Bubble(bub_x, bub_y, 2*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 2*BUBBLE_DEFAULT_VY, COLOR(225,180,255)));
                      break;
                 }
                 else if(bubbles[i].get_radius() == 2*BUBBLE_DEFAULT_RADIUS)
                 {
                      bubbles.erase(bubbles.begin() + i);
                      bullets.erase(bullets.begin() + j);
                      bubbles.push_back(Bubble(bub_x, bub_y, BUBBLE_DEFAULT_RADIUS, -2*BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,105,180)));
                      bubbles.push_back(Bubble(bub_x, bub_y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 2*BUBBLE_DEFAULT_VY, COLOR(255,105,180)));
                      break;
                 }
                 bubbles.erase(bubbles.begin() + i);
                 bullets.erase(bullets.begin() + j);
                 break;
            }
        }
    }
    return 0;
}

vector<Bubble> create_bubbles_1()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,105,180)));
    return bubbles;
}
vector<Bubble> create_bubbles_2()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(225,180,255)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(225,180,255)));
    bubbles.push_back(Bubble(3*WINDOW_X/4.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, 2*BUBBLE_DEFAULT_VX, 2*BUBBLE_DEFAULT_VY, COLOR(225,180,255)));
    return bubbles;
}
vector<Bubble> create_bubbles_3()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 4*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(150,0,255)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, 4*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(150,0,255)));
    bubbles.push_back(Bubble(3*WINDOW_X/4.0, BUBBLE_START_Y, 4*BUBBLE_DEFAULT_RADIUS, 2*BUBBLE_DEFAULT_VX, 2*BUBBLE_DEFAULT_VY, COLOR(150,0,255)));
    return bubbles;
}



int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string level("LEVEL 1!");
    Text charlevel(WINDOW_X/2, WINDOW_Y/2, level);
    wait(100*STEP_TIME);
    level = "";
    charlevel.setMessage(level);

    string msg_cmd("Cmd: ");
    Text charPressed(20, BOTTOM_MARGIN, msg_cmd);

    string scoremsg("Score: ");
    int score = 0;
    stringstream ss;
    ss<<score;
    string s;
    ss>>s;
    Text charscore(WINDOW_X - 60, BOTTOM_MARGIN, scoremsg + s);
    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles1 = create_bubbles_1();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    int health = 3;
    string Health("Health: 3/3");
    Text charhealth(WINDOW_X - 35, TOP_MARGIN, Health);

    string printlevel("LEVEL: 1/3");
    Text charprintlevel(WINDOW_X/2, BOTTOM_MARGIN, printlevel);
    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        int get = move_bubbles(bubbles1, shooter, bullets, score);
        stringstream ss;
        ss<<score;
        string s;
        ss>>s;
        charscore.setMessage(scoremsg + s);
        if(get == 1)
        {
             if(health == 1) {cout<< "GAME OVER!"<<endl; return 0;}
             else if(health == 2)
             {
                  Health = "Health: 1/3";
                  health = 1;
                  for(int i = 0; i < 30; i++)
                  {
                       move_bubbles(bubbles1, shooter, bullets, score);
                       move_bullets(bullets);
                       wait(STEP_TIME);
                       stringstream ss;
                       ss<<score;
                       string s;
                       ss>>s;
                       charscore.setMessage(scoremsg + s);
                  }
             }
             else
             {
                  Health = "Health: 2/3";
                  health = 2;
                  for(int i = 0; i < 30; i++)
                  {
                       move_bubbles(bubbles1, shooter, bullets, score);
                       move_bullets(bullets);
                       wait(STEP_TIME);
                       stringstream ss;
                       ss<<score;
                       string s;
                       ss>>s;
                       charscore.setMessage(scoremsg + s);
                  }
             }
             charhealth.setMessage(Health);
        }
        // Update the bullets

        move_bullets(bullets);

        if(bubbles1.size() == 0) break;

        wait(STEP_TIME);

    }

    level = "LEVEL_2!";
    charlevel.setMessage(level);
    wait(100*STEP_TIME);
    level = "";
    charlevel.setMessage(level);
    printlevel = ("LEVEL: 2/3");
    charprintlevel.setMessage(printlevel);

    for(int i=0; i < bullets.size(); i++){
        bullets.erase(bullets.begin() + i);
    }

    vector<Bubble> bubbles2 = create_bubbles_2();
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        int get = move_bubbles(bubbles2, shooter, bullets, score);

        stringstream ss;
        ss<<score;
        string s;
        ss>>s;
        charscore.setMessage(scoremsg + s);

        if(get == 1)
        {
             if(health == 1) {cout<< "GAME OVER!"<<endl; return 0;}
             else if(health == 2)
             {
                  Health = "Health: 1/3";
                  health = 1;
                  for(int i = 0; i < 50; i++)
                  {
                       move_bubbles(bubbles2, shooter, bullets, score);
                       move_bullets(bullets);
                       wait(STEP_TIME);
                       stringstream ss;
                       ss<<score;
                       string s;
                       ss>>s;
                       charscore.setMessage(scoremsg + s);
                  }
             }
             else
             {
                  Health = "Health: 2/3";
                  health = 2;
                  for(int i = 0; i < 50; i++)
                  {
                       move_bubbles(bubbles2, shooter, bullets, score);
                       move_bullets(bullets);
                       wait(STEP_TIME);
                       stringstream ss;
                       ss<<score;
                       string s;
                       ss>>s;
                       charscore.setMessage(scoremsg + s);
                  }
             }
             charhealth.setMessage(Health);
        }
        // Update the bullets

        move_bullets(bullets);
        if(bubbles2.size() == 0) break;

        wait(STEP_TIME);
    }

    level = "LEVEL_3!";
    charlevel.setMessage(level);
    wait(100*STEP_TIME);
    level = "";
    charlevel.setMessage(level);
    printlevel = ("LEVEL: 3/3");
    charprintlevel.setMessage(printlevel);

    for(int i=0; i < bullets.size(); i++){
        bullets.erase(bullets.begin() + i);
    }

    vector<Bubble> bubbles3 = create_bubbles_3();
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        int get = move_bubbles(bubbles3, shooter, bullets, score);

        stringstream ss;
        ss<<score;
        string s;
        ss>>s;
        charscore.setMessage(scoremsg + s);

        if(get == 1)
        {
             if(health == 1) {cout<< "GAME OVER!"<<endl; return 0;}
             else if(health == 2)
             {
                  Health = "Health: 1/3";
                  health = 1;
                  for(int i = 0; i < 30; i++)
                  {
                       move_bubbles(bubbles3, shooter, bullets, score);
                       move_bullets(bullets);
                       wait(STEP_TIME);
                       stringstream ss;
                       ss<<score;
                       string s;
                       ss>>s;
                       charscore.setMessage(scoremsg + s);
                  }
             }
             else
             {
                  Health = "Health: 2/3";
                  health = 2;
                  for(int i = 0; i < 30; i++)
                  {
                       move_bubbles(bubbles3, shooter, bullets, score);
                       move_bullets(bullets);
                       wait(STEP_TIME);
                       stringstream ss;
                       ss<<score;
                       string s;
                       ss>>s;
                       charscore.setMessage(scoremsg + s);
                  }
             }
             charhealth.setMessage(Health);
        }
        // Update the bullets

        move_bullets(bullets);
        if(bubbles3.size() == 0) break;

        wait(STEP_TIME);
    }
}
