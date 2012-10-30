#include "scale.h"

/**
 * Constructs a Scale object using the given tonic and scale template.
 *
 * @param[in]  tonic   The tonic offset of the scale. (ex. 3 = C-scale)
 * @param[in]  tmpl    The scale template to use for the scale.
 */
Scale::Scale(note_t tonic, const ScaleTemplate *tmpl)
{
    assert(tonic < 8); // tonic offset must be between 0-7 inclusive

    this->_tonic = tonic;
    this->tmpl = tmpl;

    this->pattern = tmpl->transpose(tonic); 

    this->_name = NOTE_SYMBOLS[tonic] + tmpl->name();
}

Scale::~Scale() {}

/**
 * Return the name of the scale.
 *
 * @return The name of the scale
 */
string Scale::name() const
{
    return _name;
}

/**
 * Return the tonic offset of the scale.
 *
 * @return The tonic offset
 */
note_t Scale::tonic() const
{
    return _tonic;
}

/**
 * Return the tonic offset of nth note in the scale.
 * If n>7 the scale wraps but the offset increaces
 *
 * @param[in]   n   The note number
 * @return The tonic offset
 */
int Scale::note_offset(int n) const
{
    return (n/7*12) + (12 + this->pattern.at(n % 7) - this->tonic()) % 12;
}

/**
 * Finds the given note in the scale.
 *
 * @param[in]   note   The note to find
 * @return The note index in our scale if it exists. Otherwise it returns a NULL value.
 */
size_t Scale::note_index(note_t note) const
{
    size_t i = distance(this->pattern.begin(), find(this->pattern.begin(), this->pattern.end(), note));

    // If the note isn't in the scale
    if (i == this->pattern.size()) {
        throw new string ("Note not found");
        //TODO: Do I really want to use exceptions?
    }

    return i;
}

/**
 * Helper function to finds the given note in the scale.
 *
 * @param[in]   note   The note to find
 * @return The zero-based index of the note in our scale if it exists. Otherwise it returns a NULL value.
 */
size_t Scale::note_index(Note note) const
{
    return note_index(note.note());
}

/**
 * Calculates the given interval on the scale starting at the given note.
 *
 * @param[in]   note    The note the interval should start on
 * @param[in]   deg    The degree of the interval
 * @return A Note object note on the end of the interval
 */
Note Scale::interval(Note note, int deg) const
{
    // The zero-based index of the note on the scale
    int i = this->note_index(note);

    int new_i = this->tonic() + this->note_offset(i + deg);

    int octave_delta = new_i / 12;

    Note new_note(note.octave() + octave_delta, new_i % 12);

    return new_note;
}