using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    class TurboBudgetSQLCETransactionAdapter : ITurboBudgetTransaction, ITurboBudgetSQLCETransactionAdapter
    {
        #region Private Members

        BudgetTransaction _budgetTransaction;

        #endregion
        #region Constructors

        public TurboBudgetSQLCETransactionAdapter(BudgetTransaction budgetTransaction)
        {
            _budgetTransaction = budgetTransaction;
        }

        #endregion
        #region ITurboBudgetTransaction Properties

        int ITurboBudgetTransaction.ID
        {
            get { return _budgetTransaction.ID; }
            set { _budgetTransaction.ID = value; }
        }

        ITurboBudgetAccount ITurboBudgetTransaction.Account { get; set; }
        //{
        //    get { return _budgetTransaction.AccountId; }
        //    set { _budgetTransaction.AccountId = value; }
        //}

        ITurboBudgetAccount ITurboBudgetTransaction.OtherAccount { get; set; }
        //{
        //    get { return _budgetTransaction.OtherAccountId; }
        //    set { _budgetTransaction.OtherAccountId = value; }
        //}

        decimal ITurboBudgetTransaction.Amount
        {
            get { return _budgetTransaction.Amount; }
            set { _budgetTransaction.Amount = value; }
        }

        DateTime ITurboBudgetTransaction.TimeStamp
        {
            get { return _budgetTransaction.TimeStamp; }
            set { _budgetTransaction.TimeStamp = value; }
        }

        string ITurboBudgetTransaction.Description
        {
            get { return _budgetTransaction.Description; }
            set { _budgetTransaction.Description = value; }
        }

        #endregion
        #region ITurboBudgetSQLCETransactionAdapter Properties

        BudgetTransaction ITurboBudgetSQLCETransactionAdapter.BudgetTransaction
        {
            get { return _budgetTransaction; }
        }

        #endregion
    }
}
