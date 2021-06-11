#ifndef TOUCHGFX_GUI_INCLUDE_DYNAMICDRAWABLELISTITEMS_HPP_
#define TOUCHGFX_GUI_INCLUDE_DYNAMICDRAWABLELISTITEMS_HPP_


template <class T>
class DynamicDrawableListItems : public DrawableListItemsInterface
{
public:
	DynamicDrawableListItems(int number){
		numberOfElements = number;
		elements = new T[numberOfElements];
	}

    virtual Drawable* getDrawable(int16_t index)
    {
        assert(index >= 0 && index < numberOfElements);
        return &elements[index];
    }

    /**
     * Array indexer operator.
     *
     * @param  index Zero-based index of elements to access.
     *
     * @return The indexed value.
     */
    T& operator[](int index)
    {
        assert(index >= 0 && index < numberOfElements);
        return elements[index];
    }

    virtual int16_t getNumberOfDrawables()
    {
        return numberOfElements;
    }
private:
    T* elements; ///< The array of drawables
    int numberOfElements;
};


#endif /* TOUCHGFX_GUI_INCLUDE_DYNAMICDRAWABLELISTITEMS_HPP_ */
