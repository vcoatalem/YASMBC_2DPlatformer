#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"

#define NB_ENTITY_TYPE 3

enum entity_type
{
    PLAYER,
    FOE_1,
    FOE_2
};

struct transform
{
    double height;
    double width;
    struct vector2 pos;
    struct vector2 vel;
};

struct entity
{
    enum entity_type type;
    struct transform transform;
    char is_grounded;
    //SDL thingies
};

struct entity *create_entity(enum entity_type type, struct transform transform);
struct entity *clone_entity(struct entity *entity);
void destroy_entity(struct entity *entity);
void update_entity(struct entity *entity, struct gamestate *gamestate);
int collides(struct transform tf1, struct transform tf2);

#endif /* ! ENTITY_H */
