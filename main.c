#include "Jeu.h"
int q, qq;

void inverser(int *P, int n){
    for(int i = 0; i<sizeof(P); i++)
        P[i] = 0;
    P[n] = 1;
}

char *transformer_G(char *ch, int j){
    char *sh;
    sh = malloc(21 * sizeof(char));
    for(q = 0; q<21; q++){
        if(q != 13)
            sh[q] = ch[q];
    }
    sh[13] = ch[13] - 1;
    return sh;
}
int main(int argc, char *argv[]){
    int _i, _j;
    SDL_Window *fenetre = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;
    SDL_PixelFormat *format = NULL;
    Surfaces S;
    Rectangles R;
    int statut = EXIT_FAILURE;
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        printf("Erreur au niveau de : SDL_Init(SDL_INIT_VIDEO)");
        goto Quit;
    }
    if(0 != SDL_CreateWindowAndRenderer(700, 700, SDL_WINDOW_SHOWN, &fenetre, &renderer)){
        printf("Erreur au niveau de : SDL_CreateWindowAndRenderer");
        goto Quit;
    }
    format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    if(NULL == format){
        printf("Erreur au niveau de : SDL_AllocFormat");
        goto Quit;
    }
    creer_surfaces(&S, format);//On crée les 18 surfaces qui vont changer leurs positions durant le tournoa.
    creer_rectangles(&R, -1, -1);//On crée(modifie en réalité) les rectangles dans lesquelles on va mettre les surface.
    SDL_Event event;
    SDL_bool continuer = SDL_TRUE;
    int first_time = 1, a = 1, x, y, t = -1, l = 0, u = 0, v = 0, w = 0, z = 0, zz = 0, situation = 0, avec_machine = 0, couleur = 1;
    char *image1 = "Images/0000001_1.bmp";
    char *image2 = "Images/0000002_1.bmp";
    char *image3 = "Images/0000003_1.bmp";
    char *image4 = "Images/0000004_1.bmp";
    char *image = image1;

    char *regle1 = "Images/regles1_1.bmp";
    char *regle2 = "Images/regles1_2.bmp";
    char *regle3 = "Images/regles1_3.bmp";
    char *regle4 = "Images/regles1_4.bmp";
    char *background1 = "Images/Bagrnd1_1.bmp";
    char *background2 = "Images/Bagrnd1_2.bmp";
    char *background3 = "Images/Bagrnd1_3.bmp";
    char *background4 = "Images/Bagrnd1_4.bmp";
    char *B1 = background1;
    char *B2 = background2;
    char *B3 = background3;
    char *B4 = background4;
    char *gagnant = NULL;

    while(continuer){
        SDL_WaitEvent(&event);//Appeler la fonction une seule fois ans la boucle suffit.
        if(t == -1){
            load_image(renderer, "Images/debut.bmp", surface, texture, 0);//Fenetre de début.
            SDL_Delay(750);
            t = 0;
        }
        w = 0, first_time = 1;
    retour:
        if(first_time == 2){
            x = 0, y = 0;
        }
        else
            x = event.button.x, y = event.button.y;
        if(event.type != SDL_MOUSEBUTTONUP){
            if(u >= 1 || (t != 0 && x >= 182 && x <= 481 && y >= 192 && y <= 636)){//t = 2 correspont au menu.
                if(v >= 1 || u == 1 || t == 2 || (y <= 295 && t <= 2)){//Jouer, si t == 2, il n'a pas le droit d'accéder à cette place.

                    if(v >= 1 || t == 2 || (event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && u == 0)){
                        if(v >= 1 && ((x >= 154 && x <= 589 && y >= 224 && y <= 345) || v == 2)){
                        jeu:
                            if(v != 2 && u == 1){
                                if(v == 1 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1){
                                    v = 2;//Si v == 2, alors on a cliqué maintenant sur "Jouer" dans le menu.
                                }
                                if(l == 1){
                                    load_image(renderer, "Images/joueur_3.bmp", surface, texture, 0);
                                    avec_machine = 1;
                                    l = 0;
                                }
                                else{
                                    avec_machine = 0;
                                    load_image(renderer, "Images/joueur_2.bmp", surface, texture, 0);
                                }
                            }
                            if(v == 2){
                                if(avec_machine == 1){
                                    couleur = 1;
                                    load_image(renderer, "Images/MiniMaxAlphaBeta.bmp", surface, texture, 0);
                                    SDL_WaitEvent(&event);//Un pour la clique UP.
                                    SDL_WaitEvent(&event);//Un pour la clique down.
                                    while(!(event.button.x >= 18 && event.button.x <= 544 && event.button.y >= 224 && event.button.y <= 476 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1)){
                                        SDL_WaitEvent(&event);
                                        if(event.type == SDL_QUIT)
                                            goto fermer;
                                        if(event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && event.button.y >= 125 && event.button.y <= 192){
                                            if(event.button.x >= 113 && event.button.x <= 183)
                                                couleur = 1;//La zone où on clique sur le blanc.
                                            else if(event.button.x >= 282 && event.button.x <= 352)
                                                couleur = 2;//La zone où on clique sur le noir.

                                        }
                                    }
                                    if(event.button.y >= 351){
                                        avec_machine = 2;//avec_machine = 0 quand on ne joue pas avec la machine, avec_machine = 1 quand la machine joue par minimax, avec_machine = 2 quand la machine joue par AlphaBeta.
                                    }
                                }
                                Jouer(renderer, texture, surface, image, R, S, &situation, avec_machine, couleur);
                                if(situation == 1){
                                    continuer = SDL_FALSE;
                                }
                                else if(situation == 3){
                                    t = 1, u = 0, v = 0;
                                    first_time = 2;
                                    goto retour;
                                }
                                else if(situation == 4){
                                    t = 2, u = 1, v = 1;
                                    first_time = 2;
                                    goto retour;
                                }
                                else if(situation == 5){
                                    goto retour;
                                }
                                else if(situation == 6 || situation == 7){
                                    SDL_Delay(1750);//Apres que l'un des 2 joueurs gagne, on bloque la vue pour un un peu de temps pour voir ce qui s'est passé dans le dernier mouvement qui a donné le victoire à son joueur.
                                    if(avec_machine == 0)
                                        gagnant = transformer_D("Images/Winner1.bmp", 0, 13, situation - 6);
                                    else{
                                        if((situation == 6 && couleur == 1) || (situation == 7 && couleur == 2))
                                            gagnant = "Images/you win.BMP";
                                        else
                                            gagnant = "Images/you lose.BMP";

                                    }
                                    load_image(renderer, gagnant, surface, texture, 0);
                                    if(avec_machine == 0)
                                        SDL_WaitEvent(&event);
                                    avec_machine = 0, v = 1;
                                    SDL_WaitEvent(&event);
                                    while(event.button.button != SDL_BUTTON_LEFT || event.button.clicks != 1){
                                        if(event.type == SDL_QUIT)
                                            continuer = SDL_FALSE;
                                        SDL_WaitEvent(&event);
                                    }
                                    SDL_WaitEvent(&event);
                                    goto retour;
                                }
                            }
                        }
                        else if(v >= 1 && x >= 154 && x <= 589 && y >= 345 && y <= 506){
                            l = 1;//On donne 1 à l quand on est dans le cas de J VS M, et on la rend à 0 à chaque fois(Voir en dessous de jeu).
                            goto jeu;
                        }
                            //load_image(renderer, "Images/joueur_3.bmp", surface, texture);
                        else if(v >= 1 && x >= 154 && x <= 589 && y >= 506 && y <= 634){
                            load_image(renderer, "Images/joueur_4.bmp", surface, texture, 0);
                            if(v == 1 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1){
                                first_time = 0, t = 1, u = 0, v = 0;
                                goto retour;
                            }

                        }
                        else{
                            load_image(renderer, "Images/joueur_1.bmp", surface, texture, 0);
                        }
                        u = 1;
                        t = 2;
                        if(v == 0)
                            v = 1;
                    }// u = 1 si on a dépassé le menu
                     // u = 0 signifie qu'on est encore dans le menu.
                     // t = 2 : on a dépassé le menu de la premmière fois
                     // t = 3 : on le curseur a fait son pas sur un coix, sans cliquer sur lui.
                     // On a utilisé t pour différencier entre le premier clique et les autres.
                    else if(u == 0){
                        load_image(renderer, "Images/menu2.bmp", surface, texture, 0);
                    }
                }
                else if(u == 2 || t == 3 || (y <= 414 && t <= 3)){
                    if(t == 3 || (event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && u == 0 && first_time == 1)){
                        if(x >= 442 && x <= 558 && y >= 513 && y <= 544){
                            load_image(renderer, regle2, surface, texture, 0);
                            if(t == 3 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1){
                                first_time = 0, t = 1, u = 0, v = 0, a = 1;
                                goto retour;
                            }
                        }
                        else if(a >= 2 && (x >= 436 && x <= 485 && y >= 590 && y <= 615)){
                            load_image(renderer, regle4, surface, texture, 0);
                            if(t == 3 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && w == 0){
                                regle1 = transformer_G(regle1, a);//a : regle'a'_1.bmp.
                                regle2 = transformer_G(regle2, a);
                                regle3 = transformer_G(regle3, a);
                                regle4 = transformer_G(regle4, a);
                                a -= 1;
                                w = 1;//On donne à w la valeur 1 après qu'on clique sur l'un des flèches.
                                      //Et après on lui remet la valeur 0 qui est par défaut.
                                goto retour;
                            }
                        }
                        else if(a <= 3 && (x >= 515 && x <= 569 && y >= 590 && y <= 615)){
                            load_image(renderer, regle3, surface, texture, 0);
                            if(t == 3 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && w == 0){
                                regle1 = transformer_D(regle1, a, 13, 1);
                                regle2 = transformer_D(regle2, a, 13, 1);
                                regle3 = transformer_D(regle3, a, 13, 1);
                                regle4 = transformer_D(regle4, a, 13, 1);
                                a += 1;
                                w = 1;
                                goto retour;
                            }
                        }
                        else{
                            load_image(renderer, regle1, surface, texture, 0);
                        }
                        u = 2;
                        t = 3;
                    }
                    else if(u == 0){
                        load_image(renderer, "Images/menu3.bmp", surface, texture, 0);
                    }
                }
                else if(u == 3 || t == 4 || (y <= 535 && t <= 4 && first_time == 1)){
                    if(t == 4 || (event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && u == 0)){
                        zz = 0;
                        if(u == 0 && a == 1){
                            background1 = B1;
                            background2 = B2;
                            background3 = B3;
                            background4 = B4;
                            if(z == 0){
                                z = 1;
                                goto retour;
                            }
                        }
                        if(x >= 262 && x <= 449 && y >= 384 && y <= 452){
                            load_image(renderer, background2, surface, texture, 0);
                            if(t == 4 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1){
                                for(qq = 1; qq<a; qq++)
                                    image = transformer_D(image, a, 13, 1);
                                first_time = 0, t = 1, u = 0, v = 0, a = 1;
                                goto retour;
                            }
                        }
                        else if(a <= 3 && x >= 404 && x <= 540 && y >= 468 && y <= 547){
                            if(a == 1)
                                image = image1;
                            load_image(renderer, background3, surface, texture, 0);
                            if(t == 4 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && w == 0){
                                background1 = transformer_D(background1, a, 13, 1);//a : regle'a'_1.bmp.
                                background2 = transformer_D(background2, a, 13, 1);
                                background3 = transformer_D(background3, a, 13, 1);
                                background4 = transformer_D(background4, a, 13, 1);
                                a += 1;
                                w = 1;
                                goto retour;
                            }
                        }
                        else if(a >= 2 && x >= 150 && x <= 286 && y >= 468 && y <= 547){
                            if(a == 1){
                                image = image1;
                            }
                            load_image(renderer, background4, surface, texture, 0);
                            if(t == 4 && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && w == 0){
                                background1 = transformer_G(background1, a);//a : regle'a'_1.bmp.
                                background2 = transformer_G(background2, a);
                                background3 = transformer_G(background3, a);
                                background4 = transformer_G(background4, a);
                                a -= 1;
                                w = 1;
                                goto retour;
                            }
                        }
                        else{
                            load_image(renderer, background1, surface, texture, 0);
                        }
                        u = 3;
                        t = 4;
                    }
                    else if(u == 0)
                        load_image(renderer, "Images/menu4.bmp", surface, texture, 0);
                }
                else if(y <= 636){//printf("kkkk\n");
                    if(event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1 && first_time == 1)
                        continuer = SDL_FALSE;
                    else{
                        load_image(renderer, "Images/menu5.bmp", surface, texture, 0);
                    }
                }
            }
            else if((t == 0 || t == 1) && u == 0){
                load_image(renderer, "Images/menu1.bmp", surface, texture, 0);
                t = 1;
            }
        }
        if(event.type == SDL_QUIT){//Sélectionner "fermer".
        fermer:
            continuer = SDL_FALSE;
        }
    }
    statut = EXIT_SUCCESS;
Quit:
    if(NULL != format)
        SDL_FreeFormat(format);
    if(NULL != surface)
        SDL_FreeSurface(surface);
    for(_i = 0; _i < 2; _i ++){
        for(_j = 0; _j < 9; _j++){
            if(NULL != S.No[_i][_j])
                SDL_FreeSurface(S.No[_i][_j]);
        }
    }
    if(NULL != texture)
    SDL_DestroyTexture(texture);
    if(NULL != renderer)
    SDL_DestroyRenderer(renderer);
    if(NULL != fenetre)
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    return statut;
}
