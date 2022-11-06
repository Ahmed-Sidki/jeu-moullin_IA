#include "loi du jeu.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

typedef struct{
    int x, y;
}coordonnees;
typedef struct{
    SDL_Surface *No[2][10];
}Surfaces;
typedef struct{//Les rectangles dans lequelles on met les pions.
    SDL_Rect R_In[3][8];//qui sont en train de jouer.
    SDL_Rect R_Out[2][9];//qui sont en dehors du jeu.
    SDL_Rect R_Cote[2];
}Rectangles;
typedef struct{
    Uint32 px[700 * 700];
}pixel;
typedef struct{
    pixel px[2];
}pixels;
int _h = 700;
pixels Pixels;

void load_image(SDL_Renderer *renderer,char *ch, SDL_Surface *surface, SDL_Texture *texture, int dans_jeu){
    SDL_RenderClear(renderer);
    surface = SDL_LoadBMP(ch);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    if(dans_jeu == 0)
        SDL_RenderPresent(renderer);
}

void dessiner_pion(Uint32 *px,char *couleur, int h, SDL_PixelFormat *format){
    int i, j;
    float x, y;
    int z, zz, s = 75;
    for(i = 0; i<h; i++){
        x = i;
        y = (h/2) * (1 - pow(4 * x/h - pow(2 * x/h, 2), 0.5));
        for(j = 0; j<h; j++){
            if(i<=h/2 && j<= h/2 && j>=y){
                z = (h/2 - i) * (h/2 - i) + (h/2 - j) * (h/2 - j);
                zz = h/2 - pow(z, 0.5);
                if(couleur[0] == 'B')
                    zz = zz * 256/s;
                else if(couleur[0] == 'N')
                    zz = 256 - zz * 256/s;
                if(z >= (h/2 - 2 * s) * (h/2 - 2 * s))
                    px[h*i + j] = SDL_MapRGBA(format, zz, zz, zz, 255);
                else{
                    if(couleur[0] == 'B')
                        px[h*i + j] = SDL_MapRGBA(format, 255, 255, 255, 255);
                    else
                        px[h*i + j] = SDL_MapRGBA(format, 0, 0, 0, 255);
                }
            }
            else if(i<=h/2)
                px[h*i + j] = px[h*i + h-j];
            else
                px[h*i + j] = px[h*(h-i) + j];
        }
    }
}

coordonnees position(int i, int j){
    int w = 49, h = 99, pas = 83;
    if(j == 7 || j == 0 || j == 1){
        if(i == 1)
            h += 1 * pas;
        else if(i == 2)
            h += 2 * pas;
    }
    else if(j == 3 || j == 4 || j == 5){
        if(i == 0)
            h += 6 * pas;
        if(i == 1)
            h += 5 * pas;
        if(i == 2)
            h += 4 * pas;
    }
    else
        h += 3 * pas;
    if(j == 1 || j == 2 || j == 3){
        if(i == 0)
            w += 6 * pas;
        else if(i == 1)
            w += 5 * pas;
        else if(i == 2)
            w += 4 * pas;
    }
    else if(j == 5 || j == 6 || j == 7){
        if(i == 1)
            w += 1 * pas;
        else if(i == 2)
            w += 2 * pas;
    }
    else
        w += 3 * pas;




    if(i == 0){
        if(j == 1 || j == 3)
            w += 1;
        else if(j == 2)
            w += 1;
        else if(j == 4)
            w -= 1;
        if(j == 3 || j == 4)
            h += 2;
        if(j == 6)
            h += 1;
    }
    else if(i == 1){
        if(j == 3)
            h += 2;
        else if(j == 5)
            h += 1;
        w -= 1;
        if(j == 1)
            w += 2;
        else if(j == 2)
            w += 2;
        else if(j == 3)
            w += 1;
    }
    else{
        if(j != 7 && j != 6)
            h += 1;
        if(j == 3)
            h += 2;
        else if(j == 0)
            h += 3;
        else if(j == 4)
            h -= 1;
        if(j == 7 || j == 2)
            w -= 1;
        else if(j == 0)
            w -= 1;
        else if(j == 4)
            w -= 2;
        else if(j == 6)
            w += 2;
    }
    coordonnees C;
    C.x = w;
    C.y = h;
    return C;
}

void creer_surfaces(Surfaces *S, SDL_PixelFormat *format){
    Uint32 px1[700*700];
    int i, j;
    for(i = 0; i < 2; i ++){
        for(j = 0; j < 9; j++){
            if(i == 0)
                dessiner_pion(Pixels.px[i].px, "B", _h, format);
            else if(i == 1)
                dessiner_pion(Pixels.px[i].px, "N", _h, format);
            S->No[i][j] = SDL_CreateRGBSurfaceFrom(Pixels.px[i].px, _h, _h, 32, _h * sizeof(Uint32), format->Rmask, format->Gmask, format->Bmask, format->Amask);
        }
    }
    dessiner_pion(Pixels.px[0].px, "B", _h, format);
    S->No[0][9] = SDL_CreateRGBSurfaceFrom(Pixels.px[0].px, _h, _h, 32, _h * sizeof(Uint32), format->Rmask, format->Gmask, format->Bmask, format->Amask);
    dessiner_pion(Pixels.px[1].px, "N", _h, format);
    S->No[1][9] = SDL_CreateRGBSurfaceFrom(Pixels.px[1].px, _h, _h, 32, _h * sizeof(Uint32), format->Rmask, format->Gmask, format->Bmask, format->Amask);
}

void creer_rectangles(Rectangles *R, int _i, int _j){//_i, _j sont les positions du dernier mouvement quand on veut faire déplacer un pion.
    int i, j;
    for(i = 0; i<3; i++){
        for(j = 0; j<8; j++){
            SDL_Rect _R = {position(i, j).x - 27, position(i, j).y - 27, 27*2, 27*2};;
            if(i == _i && j == _j){
                _R.x = position(i, j).x - 33;
                _R.y = position(i, j).y - 33;
                _R.h = 33*2;
                _R.w = 33*2;
            }
            R->R_In[i][j] = _R;
        }
    }
    int w = 49 - 27, h = 16, pas = 70;
    for(i = 0; i<2; i++){
        for(j = 0; j<9; j++){
            SDL_Rect _R = {w + j * pas, h * ((i + 1)%2) + (700 - h - 27 * 2) * (i%2), 27*2, 27*2};
            R->R_Out[i][j] = _R;
        }
    }
    for(i = 0; i<2; i++){
        SDL_Rect _R = {623 - 35, 400 + 100 * i - 35, 35*2, 35*2};
        R->R_Cote[i] = _R;

    }
}

void Afficher(etat et, Surfaces S, Rectangles R, SDL_Renderer *renderer, SDL_Texture *texture, char *image, int i1, int j1){
    SDL_RenderClear(renderer);
    SDL_Surface *surface;
    load_image(renderer, image, surface, texture, 1);
    int i, j, _i, _j, _j1 = 8, _j2 = 8;
    for(i = 0; i<3; i++){
        for(j = 0; j<8; j++){
            if(et.T[i][j] == P1){
                _i = 0;
                _j1--;
                _j = _j1;
            }
            else if(et.T[i][j] == P2){
                _i = 1;
                _j2 --;
                _j = _j2;
            }
            if(et.T[i][j] != 0){
                texture = SDL_CreateTextureFromSurface(renderer, S.No[_i][_j + 1]);
                creer_rectangles(&R, i1, j1);
                SDL_RenderCopy(renderer, texture, NULL, &R.R_In[i][j]);
            }
        }
    }
    _j2 = 9 - et.NU;
    if(et.joueur == J1)
        _j1 = 9 - et.NU;
    if(et.joueur == J2)
        _j1 = 9 - 1 - et.NU;
    for(j = 0; j<_j1; j++){
        texture = SDL_CreateTextureFromSurface(renderer, S.No[0][j]);
        SDL_RenderCopy(renderer, texture, NULL, &R.R_Out[0][j]);
    }
    for(j = 0; j<_j2; j++){
        texture = SDL_CreateTextureFromSurface(renderer, S.No[1][j]);
        SDL_RenderCopy(renderer, texture, NULL, &R.R_Out[1][j]);
    }
    i = Pion(et) - 1;
    if(et.manger == 1)
        i = 2 - Pion(et);
    texture = SDL_CreateTextureFromSurface(renderer, S.No[i][9]);
    SDL_RenderCopy(renderer, texture, NULL, &R.R_Cote[i]);
    SDL_RenderPresent(renderer);
}

int determiner_position_du_clique(int x, int y, Rectangles R){
    int i, j;
    for(i = 0; i<3; i++){
        for(j = 0; j<8; j++){
            if(R.R_In[i][j].x <= x && R.R_In[i][j].x + R.R_In[i][j].w >= x && R.R_In[i][j].y <= y && R.R_In[i][j].y + R.R_In[i][j].h >= y)
                return 10*i + j;
        }
    }
}

char determiner_direction(int i, int j, int _i, int _j){
    if(i == (_i + 1)%8 && j == _j)
        return H;
    else if(i == (_i + 7)%8 && j == _j)
        return BB;
    else if(i == _i && j == (_j + 1)%8)
        return G;
    else if(i == _i && j == (_j + 7)%8)
        return D;
}

char *transformer_D(char *ch, int j, int m, int n){
    int q;
    char *sh;
    sh = malloc(21 * sizeof(char));
    for(q = 0; q<21; q++){
        if(q != m)
            sh[q] = ch[q];
    }
    sh[m] = ch[m] + n;
    return sh;
}

void Non_Jouer(SDL_Event event, int *situation, SDL_bool *continuer, char **image){//La fonction qui va s'occuper du menu quand on est en train de jouer.
    if(event.button.x >= 584 && event.button.y >= 130){
        if(event.button.y <= 173){
            *situation = -2;
            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                *continuer = SDL_FALSE;
                *situation = 2;
            }
            *image = transformer_D(*image, 0, 15, 1);

        }
        else if(event.button.y <= 225){
            *situation = -3;
            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                *continuer = SDL_FALSE;
                *situation = 3;
            }
            *image = transformer_D(*image, 0, 15, 2);
        }
        else if(event.button.y <= 276){
            *situation = -4;
            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                *continuer = SDL_FALSE;
                *situation = 4;
            }
            *image = transformer_D(*image, 0, 15, 3);
        }
        else if(event.button.y <= 324){
            *situation = -5;
            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                *continuer = SDL_FALSE;
                *situation = 5;
            }
            *image = transformer_D(*image, 0, 15, 4);
        }
    }
}


void Jouer(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Surface *surface, char *image, Rectangles R, Surfaces S, int *situation, int avec_machine, int couleur){
    char *immage = image;
    *situation = 0;
    etat et = {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, J1, 0, 0, 0, -1, 0, {0, 0}, {0, 0}, 0, 0};

    Inserer(et, &chemin);
    etat _et = et;
    noeud *Noeud = malloc(sizeof(noeud));
    int _x, _y, _i, _j, i1, j1, _k = 0;
    SDL_Event event;
    SDL_bool continuer = SDL_TRUE;
    load_image(renderer, image, surface, texture, 1);//Importer le background.
    Afficher(et, S, R, renderer, texture, image, -1, -1);
    while(continuer && !tournoa_termine(et)){
        if(etatSolution(et, chemin.fin->Et)){
            chemin.debut = chemin.fin = NULL;
            Inserer(et, &chemin);
        }
        m = 0;
        while((m == 0 || et.possible == -2) && continuer){
        debutt:
            SDL_WaitEvent(&event);
            image = immage;
            Non_Jouer(event, situation, &continuer, &image);
            if((event.button.button != SDL_BUTTON_LEFT && event.type != SDL_MOUSEBUTTONDOWN) && event.button.x >= 584 && event.button.y >= 130 && event.button.y <= 324){
                load_image(renderer, image, surface, texture, 1);
                if(_k != *situation || _k == 0){
                    Afficher(et, S, R, renderer, texture, image, -1, -1);
                    _k = *situation;
                }
            }
            else{
                _k = 0;
                if(*situation < 0){
                    *situation = 0;
                    load_image(renderer, image, surface, texture, 1);
                    Afficher(et, S, R, renderer, texture, image, -1, -1);
                    SDL_RenderPresent(renderer);
                }
            }
        A_l_autre_joueur:
            et.manger = 0;
            if(Pion(et) == couleur || avec_machine == 0){
                if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                    image = immage;
                    Non_Jouer(event, situation, &continuer, &image);
                    if(*situation > 0)
                        goto fin;
                    n = determiner_position_du_clique(event.button.x, event.button.y, R);
                    _i = n/10;
                    _j = n%10;
                    ii = -1; jj = -1; dep = N;
                    if(Phase(et) == 1){
                        et.dern_mvt[0] = _i;
                        et.dern_mvt[1] = _j;
                    }
                    else if(Phase(et) == 2){
                        if(et.T[_i][_j] == Pion(et)){
                            Afficher(et, S, R, renderer, texture, image, _i, _j);
                        deplacer:
                            SDL_WaitEvent(&event);
                            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                                image = immage;
                                Non_Jouer(event, situation, &continuer, &image);
                                if(*situation > 0)
                                    goto fin;
                                n = determiner_position_du_clique(event.button.x, event.button.y, R);
                                dep = determiner_direction(_i, _j, n/10, n%10);
                            }
                            else
                                goto deplacer;
                        }
                    }
                    else if(Phase(et) == 3){
                        if(et.T[_i][_j] == Pion(et)){
                            Afficher(et, S, R, renderer, texture, image, _i, _j);
                        deplacer2:
                            SDL_WaitEvent(&event);
                            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                                image = immage;
                                if(*situation > 0){
                                    goto fin;
                                }
                                Non_Jouer(event, situation, &continuer, &image);
                                n = determiner_position_du_clique(event.button.x, event.button.y, R);
                                ii = n/10;
                                jj = n%10;
                                et.dern_mvt[0] = ii;
                                et.dern_mvt[1] = jj;
                            }
                            else
                                goto deplacer2;
                        }
                    }
                    et = placer_un_pion(et, _i, _j, ii, jj, dep);
                    m ++;
                    Afficher(et, S, R, renderer, texture, image, -1, -1);
                    Inserer(et, &chemin);
                    if(et.manger == 1){
                        m = 0;
                        et.manger = 0;
                        while(m == 0 && continuer == SDL_TRUE){//Si on donne une position qui est incorrecte, il faut qu'on ressaye jusqu'à ce qu'on donne une bonne position.
                            SDL_WaitEvent(&event);
                            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                                image = immage;
                                if(*situation > 0){
                                    goto fin;
                                }
                                Non_Jouer(event, situation, &continuer, &image);
                                n = determiner_position_du_clique(event.button.x, event.button.y, R);
                                ii = n/10;
                                jj = n%10;
                                et = choisir_apres_le_deplacement_du_pion(et, _i, _j, ii, jj, N);
                                if(et.possible == 2){
                                    et.manger = 0;
                                    m ++;
                                }
                            }
                            if(event.type == SDL_QUIT){
                                continuer = SDL_FALSE;
                                *situation = 1;
                            }
                        }
                        Afficher(et, S, R, renderer, texture, image, -1, -1);
                        Inserer(et, &chemin);
                    }
                }
            }
            else{//avec_machine = 1 ou 2, càd c'est le choix du joueur VS machine.
                SDL_Delay(1000);
                int NJ1 = et.NJ1, NJ2 = et.NJ2;
                Noeud->Et = et;
                if(avec_machine == 1)
                    Noeud = MiniMax(Noeud, 4, 4, et.joueur);
                else if(avec_machine == 2)
                    Noeud = AlphaBeta(Noeud, 4, 4, et.joueur, INT_MIN, INT_MAX);
                et = Noeud->successeur->Et;
                if(et.manger == 1){
                    if(et.joueur == J2)
                        et.NJ2 -= 1;
                    else
                        et.NJ1 -= 1;
                }
                Afficher(et, S, R, renderer, texture, image, -1, -1);
                if(et.PasSortir == 1)
                    et.PasSortir = 2;
                if(tournoa_termine(et))
                    goto Tr_terminee;
                goto A_l_autre_joueur;
                SDL_WaitEvent(&event);
            }
            if(event.type == SDL_QUIT){//Sélectionner "fermer".
                *situation = 1;
            fin:
                if(*situation == 2){
                    if(chemin.debut != chemin.fin)
                        et = Extraire(&chemin);
                    *situation = 0;
                    continuer = SDL_TRUE;
                    goto debutt;
                }
                else
                    continuer = SDL_FALSE;
            }
        }
    }
Tr_terminee:
    if(*situation <= 0){
        if(et.NJ1 == 2)
            *situation = 7;//Le gagnant est 1.
        else if(et.NJ2 == 2)
            *situation = 6;//Le gagnant est 2.
        else if(tournoa_termine(et) == 1)
            *situation = 8 - Pion(et);
    }
}
