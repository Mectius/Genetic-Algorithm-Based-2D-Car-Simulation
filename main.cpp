#include "raylib.h"
#include <iostream>
#include <cmath>
#include "Carai.h"

// Define ToRadians function
float ToRadians(float degrees) {
    return degrees * (PI / 180.0f);
}

std::vector<Car> cars;
int car_population_size = 15;
int generation = 0;
float fitness;

bool generetion_ended = false;

std::vector<Circle> obstacles;
int obstacles_count = 20;

float distance(float x, float xt, float y, float yt){
    return sqrt(abs(x-xt)*abs(x-xt)+abs(y-yt)*abs(y-yt));
}

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 670;

    InitWindow(screenWidth, screenHeight, "Car AI");

    SetTargetFPS(60);

    //border mask
    Texture2D ground = LoadTexture("assets/ground.png");
    ground.height*=1.2;
    ground.width*=1.2;

    //Point-B
    Texture2D pointb = LoadTexture("assets/point-b.png");
    pointb.width*=2;
    pointb.height*=2;

    //Goal Point Variables
    Rectangle goal_shape = {480,50,100,100};
    Vector2 goal_vec = {goal_shape.width/2, goal_shape.height / 2};

    //--------------------------------------------------------Creating Obstacle population
    int point=500;
    int st=5;
    for (int i=0;i<obstacles_count;i++){
        Circle obstacle = {point,420,20};
        obstacles.push_back(obstacle);
        Circle obstacle1 = {point+100,250,20};
        obstacles.push_back(obstacle1);
        Circle obstacle2 = {point-150,300+st,20};
        obstacles.push_back(obstacle2);
        Circle obstacle3 = {750+st,point-250,20};
        obstacles.push_back(obstacle3);
        Circle obstacle4 = {350,point-350,20};
        obstacles.push_back(obstacle4);
        Circle obstacle5 = {600,point-400,20};
        obstacles.push_back(obstacle5);
        Circle obstacle6 = {850,point-100,20};
        obstacles.push_back(obstacle6);
        Circle obstacle7 = {600,point-70,20};
        obstacles.push_back(obstacle7);
        Circle obstacle8 = {850,point+50,20};
        obstacles.push_back(obstacle8);
        Circle obstacle9 = {600,point+50,20};
        obstacles.push_back(obstacle9);
        Circle obstacle10 = {350,point-500,20};
        obstacles.push_back(obstacle10);
        Circle obstacle11 = {600,point-500,20};
        obstacles.push_back(obstacle11);
        point+=5;
        st+=5;
    }


    //Creating car population
    for (int i=0;i<car_population_size;i++){
        Car car;
        int area_x1 = 770;
        int area_x2 = 720;
        int area_y1 = 600;
        int area_y2 = 500;
        int reaction_distance=70;
        int max_rot_speed=40;
        car.core.x=rand()%(area_x1-area_x2+1)+area_x2;
        car.core.y=rand()%(area_y1-area_y2+1)+area_y2;
        car.B_reaction=rand()%reaction_distance+10;
        car.C_reaction=rand()%reaction_distance+10;
        car.D_reaction=rand()%reaction_distance+10;
        car.E_reaction=rand()%reaction_distance+10;
        car.B_rot_speed=rand()%max_rot_speed+1;
        car.C_rot_speed=rand()%max_rot_speed+1;
        car.D_rot_speed=rand()%max_rot_speed+1;
        car.E_rot_speed=rand()%max_rot_speed+1;
        for (int i=0;i<1;i++){  // !!!!!  You may be change this for distences count. Nowly it's for  goal point 
            car.A_S.push_back(1000);   //1000 was randomly selected. because of creating new places
        }
        for (int i=0;i<obstacles.size();i++){
            car.B_S.push_back(1000);
        }
        for (int i=0;i<obstacles.size();i++){
            car.C_S.push_back(1000);
        }
        for (int i=0;i<obstacles.size();i++){
            car.D_S.push_back(1000);
        }
        for (int i=0;i<obstacles.size();i++){
            car.E_S.push_back(1000);
        }
        cars.push_back(car);
    }

    while (!WindowShouldClose()) {
        // Draw
        BeginDrawing();
    
        //Draw Ground
        DrawTexture(ground, screenWidth/2 - ground.width/2, screenHeight/2 - ground.height/2, WHITE);

        //Draw Generation Number
        DrawText("GEN:",950,30,40,BLACK);
        DrawText(TextFormat("%i",generation),1050,20,50,BLACK);

        for (int c=0;c<cars.size();c++){
            for (int ob=0;ob<obstacles.size();ob++){
                //cars[c].A_S[ob] = (distance(cars[c].core.x,obstacles[ob].x,cars[c].core.y,obstacles[ob].y)); !!!! THIS IS DECTIVE for distances
                cars[c].B_S[ob] = (distance(cars[c].Bx,obstacles[ob].x,cars[c].By,obstacles[ob].y));
                cars[c].C_S[ob] = (distance(cars[c].Cx,obstacles[ob].x,cars[c].Cy,obstacles[ob].y));
                cars[c].D_S[ob] = (distance(cars[c].Dx,obstacles[ob].x,cars[c].Dy,obstacles[ob].y));
                cars[c].E_S[ob] = (distance(cars[c].Ex,obstacles[ob].x,cars[c].Ey,obstacles[ob].y));

                //------------Disable car
                if (cars[c].B_S[ob] < obstacles[ob].radius + 5){
                    cars[c].enabled=0;
                }
                if (cars[c].C_S[ob] < obstacles[ob].radius + 5){
                    cars[c].enabled=0;
                }
                if (cars[c].D_S[ob] < obstacles[ob].radius + 5){
                    cars[c].enabled=0;
                }
                if (cars[c].E_S[ob] < obstacles[ob].radius + 5){
                    cars[c].enabled=0;
                }

                if (cars[c].core.y < goal_shape.y + goal_shape.height-10 && cars[c].core.x<goal_shape.x+.5*goal_shape.width+10 && cars[c].core.x>goal_shape.x-.5*goal_shape.width-10) {
                    cars[c].enabled=0;
                }

                if (cars[c].core.y > screenHeight-20) {
                    cars[c].enabled=0;
                }
                if (cars[c].core.x > screenWidth-20) {
                    cars[c].enabled=0;
                }
                if (cars[c].core.y < 20) {
                    cars[c].enabled=0;
                }
                if (cars[c].core.x < 20) {
                    cars[c].enabled=0;
                }

                //Reactions
                if (cars[c].B_S[ob] < cars[c].B_reaction){
                    cars[c].rotation+=-15+cars[c].B_rot_speed;
                }
                if (cars[c].C_S[ob] < cars[c].C_reaction){
                    cars[c].rotation+=-20+cars[c].C_rot_speed;
                }
                if (cars[c].D_S[ob] < cars[c].D_reaction){
                    cars[c].rotation+=-20+cars[c].D_rot_speed;
                }
                if (cars[c].E_S[ob] < cars[c].E_reaction){
                    cars[c].rotation+=-15+cars[c].E_rot_speed;
                }

            }

            //Disabled car
            if (cars[c].enabled==0) {
                //Fitnessing
                cars[c].A_S[0] = (distance(cars[c].core.x,goal_shape.x,cars[c].core.y,goal_shape.y));
                cars[c].fitness = (0-(cars[c].A_S[0])/100+10)*(0-(cars[c].A_S[0])/100+10)/5;  //set Fitness number
                if (cars[c].fitness>=14){
                    cars[c].fitness*=cars[c].fitness%14+1;
                }
                //std::cout<<cars[c].fitness<<std::endl;
            }

            //Enabled Car
            if (cars[c].enabled==1) {
                float moveX = cars[c].movementSpeed * GetFrameTime() * cos(ToRadians(cars[c].rotation));
                float moveY = cars[c].movementSpeed * GetFrameTime() * sin(ToRadians(cars[c].rotation));
                cars[c].core.x += moveX;
                cars[c].core.y += moveY;
            }

            DrawRectanglePro(cars[c].core,cars[c].vec,cars[c].rotation,RED);

            //Sensor points. That's complex but works. Don't change anything
            cars[c].Ax=cars[c].core.x;
            cars[c].Ay=cars[c].core.y;
            DrawCircle(cars[c].Ax,cars[c].Ay,2,BLACK); //Center A
            DrawText("A",cars[c].Ax+3,cars[c].Ay,10,GREEN);
            DrawText(TextFormat("%i",cars[c].fitness),cars[c].Ax,cars[c].Ay+5,10,ORANGE);

            cars[c].Dx=cars[c].core.x+15*sin(ToRadians(cars[c].rotation));
            cars[c].Dy=cars[c].core.y-15*cos(ToRadians(cars[c].rotation)); //Left D
            DrawText("D",cars[c].Dx+3,cars[c].Dy,10,BLUE);
            DrawCircle(cars[c].Dx,cars[c].Dy,2,BLACK); 

            cars[c].Ex=cars[c].core.x-15*sin(ToRadians(cars[c].rotation));
            cars[c].Ey=cars[c].core.y+15*cos(ToRadians(cars[c].rotation)); //Right E
            DrawCircle(cars[c].Ex,cars[c].Ey,2,BLACK);
            DrawText("E",cars[c].Ex+3,cars[c].Ey,10,BLUE);

            cars[c].Cx=cars[c].Ex+30*cos(ToRadians(cars[c].rotation));
            cars[c].Cy=cars[c].Ey+30*sin(ToRadians(cars[c].rotation)); //Right Bottom C
            DrawCircle(cars[c].Cx,cars[c].Cy,2,BLACK);
            DrawText("C",cars[c].Cx+3,cars[c].Cy,10,BLUE);

            cars[c].Bx=cars[c].Dx+30*cos(ToRadians(cars[c].rotation));
            cars[c].By=cars[c].Dy+30*sin(ToRadians(cars[c].rotation)); //Left Bottom B
            DrawCircle(cars[c].Bx,cars[c].By,2,BLACK);
            DrawText("B",cars[c].Bx+3,cars[c].By,10,BLUE);
        }

        //Enable with keyboard
        if (IsKeyPressed(KEY_ENTER)){
            for (int c=0;c<cars.size();c++){
                cars[c].enabled=0;
            }
        }
        //check
        int for_count1 = 0;
        for (int c=0;c<cars.size();c++){
            if (cars[c].enabled==1){
                break;
            }
            for_count1++;
        }

        if (for_count1 == cars.size()){
            generetion_ended=true;     
        }

        if (generetion_ended==true){
            std::vector<Car> parents;
            for (int i=0;i<cars.size();i++){
                for (int v=0;v<cars[i].fitness;v++){
                    parents.push_back(cars[i]);
                }
            }
            std::vector<couple> couples;
            for (int i=0;i<car_population_size;i++){
                couple cple;
                cple.a = parents[rand()%parents.size()];
                cple.b = parents[rand()%parents.size()];
                couples.push_back(cple);
            }
            cars.clear();
            for (int i=0;i<car_population_size;i++){
                Car car;
                car.B_reaction = (couples[i].a.B_reaction+couples[i].b.B_reaction)/2+rand()%1;
                car.B_rot_speed = (couples[i].a.B_rot_speed+couples[i].b.B_rot_speed)/2+rand()%1;

                car.C_reaction = (couples[i].a.C_reaction+couples[i].b.C_reaction)/2-rand()%1;
                car.C_rot_speed = (couples[i].a.C_rot_speed+couples[i].b.C_rot_speed)/2-rand()%1;

                car.D_reaction = (couples[i].a.D_reaction+couples[i].b.D_reaction)/2+rand()%1;
                car.D_rot_speed = (couples[i].a.D_rot_speed+couples[i].b.D_rot_speed)/2+rand()%1;

                car.E_reaction = (couples[i].a.E_reaction+couples[i].b.E_reaction)/2-rand()%1;
                car.E_rot_speed = (couples[i].a.E_rot_speed+couples[i].b.E_rot_speed)/2-rand()%1;

                int area_x1 = 770;
                int area_x2 = 720;
                int area_y1 = 600;
                int area_y2 = 500;
                car.core.x=rand()%(area_x1-area_x2+1)+area_x2;
                car.core.y=rand()%(area_y1-area_y2+1)+area_y2;

                for (int i=0;i<1;i++){  // !!!!!  You may be change this for distences count. Nowly it's for  goal point 
                    car.A_S.push_back(1000);   //1000 was randomly selected. because of creating new places
                }
                for (int i=0;i<obstacles.size();i++){
                    car.B_S.push_back(1000);
                }
                for (int i=0;i<obstacles.size();i++){
                    car.C_S.push_back(1000);
                }
                for (int i=0;i<obstacles.size();i++){
                    car.D_S.push_back(1000);
                }
                for (int i=0;i<obstacles.size();i++){
                    car.E_S.push_back(1000);
                }
                cars.push_back(car);
            }
            generation+=1;
            generetion_ended=false;
        }

        //Drawing obstacles  COLLIDER OF BORDER
        /*
        for (int ob=0;ob<obstacles.size();ob++){
            DrawCircle(obstacles[ob].x,obstacles[ob].y,obstacles[ob].radius,GRAY);
        }
        */

        //Draw Goal Point
        DrawRectanglePro(goal_shape,goal_vec,0,LIGHTGRAY);
        DrawTexture(pointb,430,20,WHITE);
        DrawText("Press ENTER for maunal control", 50, 550, 30, WHITE);

        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}