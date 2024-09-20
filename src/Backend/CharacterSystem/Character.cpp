#include "Character.h"
#include  "SqaureManager.h"
namespace Cold
{
    CharacterInfo::CharacterInfo(CharacterDataSPtr data, glm::vec2 scale , glm::vec2 position) 
    : data(data), current_scale(scale), current_position(position)
    {
    }

    CharacterInfo::~CharacterInfo()
    {
    }



} // namespace Cold
