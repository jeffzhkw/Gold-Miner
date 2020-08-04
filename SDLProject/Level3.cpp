//
//  Level3.cpp
//  GoldMiner
//
//  Created by Kewei Zhang on 8/3/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level3.h"

#define LEVEL3_ENEMY_COUNT 1//TODO: Decide number
#define LEVEL3_MINES_COUNT 5
#define TARGET_SCORE 1000

GLuint fontTextureID_3;

void Level3::Initialize() {
    
    state.nextScene = -1;
    state.score = 0;
    
    //Initiaize Hook/player - default position
    state.hook = new Entity();
    state.hook->textureID = Util::LoadTexture("hook.png");
    state.hook->entityType = HOOK;
    state.hook->position = glm::vec3(0, 2, 0);
    state.hook->speed = 2;
    //Initialize MineCart
    state.mineCart = new Entity();
    state.mineCart->textureID = Util::LoadTexture("mineCart.png");
    state.mineCart->entityType = MINE_CART;
    state.mineCart->position = glm::vec3(state.hook->position.x+0.1, 3, 0);
    
    //Enemies
    GLuint pigTextureID = Util::LoadTexture("pig.png");
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = pigTextureID;
        state.enemies[i].position = glm::vec3(-6,-1,0);//TODO: design enemy position
        state.enemies[i].speed = 1;
    }
    
    //Mines
    state.mines = new Entity[LEVEL3_MINES_COUNT];
    // ------------------------------------
    //Initialize gold(type1) - value 100
    
    state.mines[0].entityType = MINE;
    state.mines[0].position = glm::vec3(2, 0, 0);
    state.mines[0].weight = 1;
    state.mines[0].value= 100;
    state.mines[0].textureID = Util::LoadTexture("gold1.png");

    state.mines[1].entityType = MINE;
    state.mines[1].position = glm::vec3(-2, 0, 0);
    state.mines[1].weight = 1;
    state.mines[1].value= 100;
    state.mines[1].textureID = Util::LoadTexture("gold1.png");
    
    state.mines[2].entityType = MINE;
    state.mines[2].position = glm::vec3(-1.5f, 1, 0);
    state.mines[2].weight = 1;
    state.mines[2].value= 100;
    state.mines[2].textureID = Util::LoadTexture("gold1.png");
    
    // ------------------------------------
    //Initialize gold(type2) - value 200
    
    state.mines[3].entityType = MINE;
    state.mines[3].position = glm::vec3(3.0f, -1, 0);
    state.mines[3].weight = 2;
    state.mines[3].value= 200;
    state.mines[3].textureID = Util::LoadTexture("gold2.png");
    
    // ------------------------------------
    //Initialize stone - value 20
    
    state.mines[4].entityType = MINE;
    state.mines[4].position = glm::vec3(0.5f, 0, 0);
    state.mines[4].weight = 3;
    state.mines[4].value= 20;
    state.mines[4].textureID = Util::LoadTexture("stone.png");
    
   
     //Initialize life
    state.lives = new Entity();
    state.lives->entityType = LIFE;
    state.lives->position = glm::vec3(3.7, 3.3, 0);
    state.lives->textureID = Util::LoadTexture("hook.png");
    state.lives->number = 3;
    
    //Initialize the text
    fontTextureID_3 = Util::LoadTexture("font2.png");
    
    
}

#define FIXED_TIMESTEP 0.0166666f
void Level3::Update(float deltaTime) {
    state.hook->Update(deltaTime, state.hook, state.enemies, LEVEL3_ENEMY_COUNT, state.mines, LEVEL3_MINES_COUNT);
    state.mineCart->Update(deltaTime, state.hook, state.enemies, LEVEL3_ENEMY_COUNT, state.mines, LEVEL3_MINES_COUNT);
    state.lives->Update(deltaTime, state.hook, state.enemies, LEVEL3_ENEMY_COUNT, state.mines, LEVEL3_MINES_COUNT);
    
    for (int i = 0; i < LEVEL3_MINES_COUNT; i++){
        state.mines[i].Update(deltaTime, state.hook, state.enemies, LEVEL3_ENEMY_COUNT, state.mines, LEVEL3_MINES_COUNT);
    }
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.hook, state.enemies, LEVEL3_ENEMY_COUNT, state.mines, LEVEL3_MINES_COUNT);
    }
    //add Score functionality;
    if (state.hook->keepMoving == true){
        state.score += state.hook->hookValue;
    }
    state.hook->hookValue = 0;
    
    //Lose Condition - all mines disppear but not reach the target score
    int inActive = 0;
    
    for (int i = 0; i < LEVEL3_MINES_COUNT; i++) {
        if (!state.mines[i].isActive) inActive += 1;
    }
    
    if (inActive == LEVEL3_MINES_COUNT && state.score < TARGET_SCORE) {
        std::cout << "Lose One Life" << std::endl;
        loseLife = true;
        state.nextScene = 1;
        state.score = 0;
    }
    
    
    
    //switch scene condition
    if (state.score >= TARGET_SCORE){
        std::cout << "Pass Level3" << std::endl;
        //state.nextScene = ;
    }
    
    if(state.hook->loseLife) {
        std::cout << "Lose One Life" << std::endl;
        loseLife = true;
        state.nextScene = 3;
        state.score = 0;
    }
    
}



void Level3::Render(ShaderProgram *program) {
    state.mineCart->Render(program);
    state.hook->Render(program);
    state.lives->Render_Life(program);
    
    for (int i = 0; i < 3; i++){
        state.mines[i].Render_Gold1(program);
    }
    
    state.mines[3].Render_Gold2(program);
    state.mines[4].Render_Stone(program);
    
    
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++){
        state.enemies[i].Render_Enemy(program);
    }
    
    Util::DrawText(program, fontTextureID_3, "LEVEL 3", 0.5f, -0.2f, glm::vec3(-0.75,3.35,0));
    Util::DrawText(program, fontTextureID_3, "Score: " + std::to_string(state.score) , 0.25f, -0.1f, glm::vec3(-4.5, 3.4, 0));
    Util::DrawText(program, fontTextureID_3, "Target Score: " + std::to_string(TARGET_SCORE) , 0.25f, -0.1f, glm::vec3(-4.5, 3.1, 0));

    //std::cout << "Press ENTER or go through START" << std::endl;
}