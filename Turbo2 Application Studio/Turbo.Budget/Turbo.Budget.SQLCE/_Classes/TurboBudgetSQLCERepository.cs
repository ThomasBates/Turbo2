using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    public class TurboBudgetSQLCERepository : ITurboBudgetRepository
    {
        #region Private Members

        TurboBudgetSQLCEContext _context = new TurboBudgetSQLCEContext();

        #endregion
        #region ITurboBudgetRepository Properties

        IQueryable<ITurboBudgetAccount> ITurboBudgetRepository.Accounts
        {
            get
            {
                return _context.BudgetAccounts
                               .Select<BudgetAccount, ITurboBudgetAccount>(a => new TurboBudgetSQLCEAccountAdapter(a));
            }
        }

        IQueryable<ITurboBudgetTransaction> ITurboBudgetRepository.Transactions
        {
            get
            {
                return _context.BudgetTransactions
                               .Select<BudgetTransaction, ITurboBudgetTransaction>(t => new TurboBudgetSQLCETransactionAdapter(t));
            }
        }

        #endregion
        #region ITurboBudgetRepository Methods

        ITurboBudgetAccount ITurboBudgetRepository.CreateAccount()
        {
            return new TurboBudgetSQLCEAccountAdapter(new BudgetAccount());
        }

        ITurboBudgetTransaction ITurboBudgetRepository.CreateTransaction()
        {
            return new TurboBudgetSQLCETransactionAdapter(new BudgetTransaction());
        }

        void ITurboBudgetRepository.AddAccount(ITurboBudgetAccount account)
        {
            if (account is ITurboBudgetSQLCEAccountAdapter)
            {
                _context.AddToBudgetAccounts(((ITurboBudgetSQLCEAccountAdapter)account).BudgetAccount);
            }
        }

        void ITurboBudgetRepository.AddTransaction(ITurboBudgetTransaction transaction)
        {
            if (transaction is ITurboBudgetSQLCETransactionAdapter)
            {
                _context.AddToBudgetTransactions(((ITurboBudgetSQLCETransactionAdapter)transaction).BudgetTransaction);
            }
        }

        #endregion
    }
}
