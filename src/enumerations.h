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
        movie = 1,
        show = 2
    };

}

#endif // ENUMERATIONS

