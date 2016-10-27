/**
 * Structure to represent local variables (a table for registers & tempvars)
 */

#include "local_variables.h"


/**
* @fn inline bool LocalVariables::isValid() const
 * @brief      Determines if valid.
 *
 * @return     True if valid, False otherwise.
 */

/**
* @fn inline short LocalVariables::maxRegisters() const
 * @brief      Gets the count of possible registers.
 *
 * @return     The registers count.
 */

/**
 * @brief      Determines if a variable is identified to be equal to a constant.
 *
 * @fn inline bool LocalVariables::isConst(OperandVar var) const
 * @param[in]  var   The variable
 *
 * @return     True if constant, False otherwise.
 */

/**
 * @brief      Gets the labels associated to a variable.
 *
 * @fn inline const labels_t& LocalVariables::labels(OperandVar var) const
 * @param[in]  var   The variable
 *
 * @return     The labels.
 */

/**
 * @brief      Associate a new label to a variable.
 * @fn inline void LocalVariables::label(OperandVar var, Edge* e)
 *
 * @param[in]  var   The variable to associate a label to
 * @param      e     The edge to label with
 */

/**
* @fn inline void LocalVariables::clearLabels(OperandVar var)
 * @brief      Clear all labels associated to a variable.
 *
 * @param[in]  var   The variable
 */

/**
 * @brief      Determines if updated.
 *
 * @fn inline bool LocalVariables::isUpdated(OperandVar var)
 * @param[in]  var   The variable
 *
 * @return     True if updated, False otherwise.
 */

/**
* @fn inline void LocalVariables::markAsUpdated(OperandVar var)
 * @brief      Mark a variable as updated
 *
 * @param[in]  var   The variable
 */

/**
 * @fn inline void LocalVariables::resetUpdatedMarks()
 * @brief      Reset all updated flags.
 */


/**
 * @brief      Merge current LocalVariables with a provided one
 *
 * @param[in]  lv    LocalVariables to merge with
 */
void LocalVariables::merge(const LocalVariables& lv) // this = this ∩ lv
{
	for(int i = 0; i < size; i++)
	{
		if(o[i] == lv.o[i])
			u.set(i, u[i] | lv.u[i]);
		else
		{
			o[i] = Top; // TODO!! handle tops so that they are not being manipulated (T-T is not 0!!)
			u.set(i, false);
		}
		// clear labels
		delete l[i];
		l[i] = NULL;
	}
}
