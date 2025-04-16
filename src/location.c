#include "location.h"
#include "model.h"
#include "entity.h"

#include <stdlib.h>

void location_free(Location *loc) {
    if (!loc) return;

    if (loc->model) {
        model_free(loc->model);
    }

    free(loc);
}