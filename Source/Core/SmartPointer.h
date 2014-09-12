/*
 * SmartPointer.h
 *
 *  Created on: Sep 11, 2014
 *      Author: jimbob
 */

#ifndef SMARTPOINTER_H_
#define SMARTPOINTER_H_

template<typename T>
class SmartPointer
{
private:
  T*  mPointer;
public:
  SmartPointer() : mPointer(NULL) {}
  SmartPointer(T *aPointer) : mPointer(aPointer) {}
  SmartPointer(SmartPointer<T> &aPointer) { mPointer = new T(*aPointer); }
  ~SmartPointer() { if(mPointer) delete mPointer; }
  T& operator*() { return *mPointer; }
  T* operator->() { return mPointer; }
  operator T*() { return mPointer; }
  operator bool() { return mPointer != NULL; }
  bool isNULL() const { return mPointer == NULL; }
  void operator=(T *aPtr) { ReassignValue(aPtr); }
  void operator=(SmartPointer<T> &aPtr) { ReassignValue(aPtr.mPointer); }
  bool operator==(T *aPtr) { return isEqual(aPtr); }
  bool operator==(SmartPointer<T> &aPtr) { return isEqual(aPtr.mPointer); }
private:
  void ReassignValue(T *aPtr)
  {
    //if(aPtr == NULL)
      //delete mPointer;
    mPointer = aPtr;
  }
  bool isEqual(T *aPtr)
  {
    return aPtr == mPointer;
  }
};

#endif /* SMARTPOINTER_H_ */
