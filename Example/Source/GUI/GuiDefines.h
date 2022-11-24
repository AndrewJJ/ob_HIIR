#pragma once

#ifndef GUI_BASE_SIZE
 #define GUI_BASE_SIZE 30.0
#endif

#ifndef GUI_BASE_SIZE_I
#define GUI_BASE_SIZE_I static_cast<int> (GUI_BASE_SIZE)
#endif

#ifndef GUI_BASE_SIZE_F
#define GUI_BASE_SIZE_F static_cast<float> (GUI_BASE_SIZE)
#endif

#ifndef GUI_BASE_SIZE_PX
#define GUI_BASE_SIZE_PX Grid::Px (GUI_BASE_SIZE_I)
#endif

#ifndef GUI_SIZE
#define GUI_SIZE(N) GUI_BASE_SIZE * (N) // Note that the brackets around the N are needed so that any calculations within the argument are performed correctly
#endif

#ifndef GUI_SIZE_I
#define GUI_SIZE_I(N) static_cast<int> (GUI_SIZE(N))
#endif

#ifndef GUI_SIZE_F
#define GUI_SIZE_F(N) static_cast<float> (GUI_SIZE(N))
#endif

#ifndef GUI_SIZE_PX
#define GUI_SIZE_PX(N) Grid::Px (GUI_SIZE_I(N))
#endif

#ifndef GUI_BASE_GAP
#define GUI_BASE_GAP 5.0
#endif

#ifndef GUI_BASE_GAP_I
#define GUI_BASE_GAP_I static_cast<int> (GUI_BASE_GAP)
#endif

#ifndef GUI_BASE_GAP_F
#define GUI_BASE_GAP_F static_cast<float> (GUI_BASE_GAP)
#endif

#ifndef GUI_BASE_GAP_PX
#define GUI_BASE_GAP_PX Grid::Px (GUI_BASE_GAP_I)
#endif

#ifndef GUI_GAP
#define GUI_GAP(N) GUI_BASE_GAP * (N) // Note that the brackets around the N are needed so that any calculations within the argument are performed correctly
#endif

#ifndef GUI_GAP_I
#define GUI_GAP_I(N) static_cast<int> (GUI_GAP(N))
#endif

#ifndef GUI_GAP_F
#define GUI_GAP_F(N) static_cast<float> (GUI_GAP(N))
#endif

#ifndef GUI_GAP_PX
#define GUI_GAP_PX(N) Grid::Px (GUI_GAP_I(N))
#endif
