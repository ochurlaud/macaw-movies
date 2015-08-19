/* Copyright (C) 2014 Macaw-Movies
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Macaw-Movies.
 *
 * Macaw-Movies is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Macaw-Movies is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Macaw-Movies.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ENUMERATIONS
#define ENUMERATIONS

namespace Macaw
{
    enum fields {
        ObjectId = Qt::UserRole,
        ObjectType = Qt::UserRole+1,
        PeopleType = Qt::UserRole+2
    };
    enum typeElement {
        None,
        isMovie,
        isPeople,
        isTag,
        isPlaylist
    };
    enum movieOrShow {
        movie = 0,
        show = 1
    };
    enum contents {
        hasMovies = 0b01,
        hasShows = 0b10
    };

}

#endif // ENUMERATIONS

