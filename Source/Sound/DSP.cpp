#include "DSP.h"

DSP::DSP(HashString const& aName) : mName(aName)
{
}

DSP::~DSP()
{
}

/**
 * @brief Get name of DSP
 * @return Name
 */
HashString const& DSP::GetName() const
{
  return mName;
}

