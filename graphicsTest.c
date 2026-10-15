/*
 * This example creates an SDL window and renderer, and then draws some points
 * to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define POPULATION 100
#define MIN_SPEED 15.0  /* move at least this many pixels per second. */
#define MAX_SPEED 30.0  /* move this many pixels per second at most. */

/* (track everything as parallel arrays instead of a array of structs,
   so we can pass the coordinates to the renderer in a single function call.) */

/* Points are plotted as a set of X and Y coordinates.
   (0, 0) is the top left of the window, and larger numbers go down
   and to the right. This isn't how geometry works, but this is pretty
   standard in 2D graphics. */

struct Agent{
    SDL_Color color;
    float angle, x, y, angleVelocity, speed;
};

static struct Agent agents[POPULATION];
static SDL_FPoint points[POPULATION];

void drawAgent(struct Agent *agent){
    SDL_SetRenderDrawColor(renderer, agent->color.r, agent->color.g, agent->color.b, agent->color.a);
    double radius = 10.0;
    SDL_FPoint circlePoints[(int)ceil(pow(radius, 2.0)*M_PI)];
    int i=0;
    for(int x=floor(-radius);x<ceil(radius);x++){
        for(int y=floor(-radius);y<ceil(radius);y++){
            double dist = sqrt(pow((double)x + 0.5, 2.0)+pow((double)y + 0.5, 2.0));
            if(dist <= radius){
                circlePoints[i].x = x+agent->x;
                circlePoints[i].y = y+agent->y;
                i++;
            }
        }
    }
    SDL_RenderPoints(renderer, circlePoints, i);
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    int i;

    SDL_SetAppMetadata("Example Epidemic agent model", "1.0", "com.example.EpidemicAgentModel");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Epidemic agent simulation", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    /* set up the data for a bunch of points. */
    for (i = 0; i < POPULATION; ++i) {
        agents[i].x = SDL_randf() * ((float) WINDOW_WIDTH);
        agents[i].y = SDL_randf() * ((float) WINDOW_HEIGHT);
        agents[i].speed = MIN_SPEED + (SDL_randf() * (MAX_SPEED - MIN_SPEED));
        agents[i].angle = SDL_randf()*2.0*M_PI;
        agents[i].angleVelocity = 0.0;
        agents[i].color.r = SDL_rand(255)+1;
        agents[i].color.g = SDL_rand(255)+1;
        agents[i].color.b = SDL_rand(255)+1;
        agents[i].color.a = 255;
        //printf("%d\n", agents[i].speed);
    }

    last_time = SDL_GetTicks();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const Uint64 now = SDL_GetTicks();
    const float elapsed = ((float) (now - last_time)) / 1000.0f;  /* seconds since last iteration */
    int i;

    /* let's move all our points a little for a new frame. */
    for (i = 0; i < POPULATION; ++i) {
        struct Agent *agent = &agents[i];
        float distanceX = elapsed * cos((double)agent->angle)*(double)agent->speed;
        float distanceY = elapsed * sin((double)agent->angle)*(double)agent->speed;
        agent->x += distanceX;
        agent->y += distanceY;
        agent->angleVelocity += (SDL_randf()*2.0-1) * 0.4/180*M_PI * elapsed;
        agent->angle += agent->angleVelocity;
        points[i].x = agent->x;
        points[i].y = agent->y;
    }
    printf("FPS: %f\n", 1.0/elapsed);

    last_time = now;

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    for(int i=0;i<POPULATION;i++){
        drawAgent(&agents[i]);
    }
    //SDL_RenderPoints(renderer, points, SDL_arraysize(points)); /* draw all the points! */

    /* You can also draw single points with SDL_RenderPoint(), but it's
       cheaper (sometimes significantly so) to do them all at once. */

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}