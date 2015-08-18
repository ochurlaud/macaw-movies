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

