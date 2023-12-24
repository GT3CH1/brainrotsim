//
// Created by gcpease on 12/24/2023.
//

#ifndef BLENDMODES_H
#define BLENDMODES_H
#include <SDL2/SDL.h>

static inline SDL_BlendMode wireframe_blend_mode =
        SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_COLOR, SDL_BLENDFACTOR_DST_COLOR, SDL_BLENDOPERATION_MINIMUM,
                                   SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDOPERATION_ADD);

static inline SDL_BlendMode normal_blend_mode =
        SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_COLOR, SDL_BLENDFACTOR_DST_COLOR, SDL_BLENDOPERATION_MAXIMUM,
                                   SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDOPERATION_ADD);
#endif // BLENDMODES_H
