using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    class TurboBudgetSQLCEAccountAdapter : ITurboBudgetAccount, ITurboBudgetSQLCEAccountAdapter
    {
        #region Private Members

        BudgetAccount _budgetAccount;

        #endregion
        #region Constructors

        public TurboBudgetSQLCEAccountAdapter(BudgetAccount budgetAccount)
        {
            _budgetAccount = budgetAccount;
        }

        #endregion
        #region ITurboBudgetAccount Properties

        int ITurboBudgetAccount.ID
        {
            get { return _budgetAccount.ID; }
            set { _budgetAccount.ID = value; }
        }
        
        string ITurboBudgetAccount.Name
        {
            get { return _budgetAccount.Name; }
            set { _budgetAccount.Name = value; }
        }

        string ITurboBudgetAccount.Caption
        {
            get { return _budgetAccount.Caption; }
            set { _budgetAccount.Caption = value; }
        }

        string ITurboBudgetAccount.Description
        {
            get { return _budgetAccount.Description; }
            set { _budgetAccount.Description = value; }
        }

        #endregion
        #region ITurboBudgetSQLCEAccountAdapter Properties

        BudgetAccount ITurboBudgetSQLCEAccountAdapter.BudgetAccount
        {
            get { return _budgetAccount; }
        }

        #endregion
    }
}
