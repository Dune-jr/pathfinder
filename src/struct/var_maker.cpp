#include "var_maker.h"

/**
 * @fn const Operand* VarMaker::new_top(void);
 * @brief      Make a new Tk
 * @return     The operand generated
 */

/**
 * @fn inline bool VarMaker::isEmpty(void) const {
 * @brief      Determines if the VarMaker is physically empty.
 * @return     True if empty, False otherwise.
 */

/**
 * @fn inline int VarMaker::length(void) const {
 * @brief      Gives the virtual length of the VarMaker
 */

/**
 * @brief      Imports the tops from the VarMaker given in parameter.
 * @param      vm    The varmaker to virtually get tops from
 * This leaves an unused empty space at the beginning of the tops.
 * For example, if we import 2 tops and this is [T0, T1, T2], this becomes [NULL, NULL, T0, T1, T2], then
 * this scales to [NULL, NULL, T2, T3, T4] (but using the same pointers)
 * [20 virtual+8].import([2 virtual+5]) = [27 virtual+8 scaled by 7]
 */
void VarMaker::import(const VarMaker& vm)
{
	const int n = vm.length();
	if(n == 0)
		return;
	// first off, increase the virtual space
	start += n;
	// now, scale the by +n
	for(tops_t::Iter i(tops); i; i++)
		tops[i]->scale(+n);
}

/**
 * @brief      Shrinks the VarMaker to a minimal size according a list of used OperandTops
 *
 * @param      vc     A variable collector, describing which variables to keep
 * @param      clean  Whether to delete variables or not
 */
void VarMaker::shrink(const VarCollector& vc, bool clean)
{
	const int n = tops.length();
	ASSERTP(vc.size() == n, "BitCollection size is different than tops size: " << vc.size() << "=/=" << n)

	int j = 0;
	for(int i = 0; i < n; i++)
	{
		if(vc[i]) // used variable
		{
			tops[j] = tops[i];
			tops[j]->scale(-i+j);
			j++;
		}
		else if(clean)
			delete tops[i]; // will not delete null pointers
			// cout << "deleting " << tops[i] << " = " << *tops[i] << endl;
	}
	DBGG("Shrinking VarMaker's " << n << " tops to " << j << ".")
	tops.shrink(j);
}

/**
 * @brief      Prints the VarMaker
 */
io::Output& VarMaker::print(io::Output& out) const
{
	out << "[" << start << " virtual";
	if(! tops.isEmpty())
		out << ", " << *tops[0] << " - " << *tops[tops.length() - 1];
	return out << "]";
}
