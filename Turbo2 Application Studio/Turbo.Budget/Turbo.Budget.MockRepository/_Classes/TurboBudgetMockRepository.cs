using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.MockRepository
{
    public class TurboBudgetMockRepository : ITurboBudgetRepository
    {
        #region Private Members

        Dictionary<int, ITurboBudgetAccount> _accounts = new Dictionary<int, ITurboBudgetAccount>();
        Dictionary<int, ITurboBudgetTransaction> _transactions = new Dictionary<int, ITurboBudgetTransaction>();

        #endregion
        #region Constructors

        public TurboBudgetMockRepository()
        {
            _accounts.Add(1, new TurboBudgetMockAccount(1, "DEPOSIT", "Deposit", "Main Deposit Account"));
            _accounts.Add(2, new TurboBudgetMockAccount(2, "AUTOMATIC", "Automatic", "Automatic Debits and Cheques"));
            _accounts.Add(3, new TurboBudgetMockAccount(3, "PAYCHEQUE", "Paycheque", "Paycheque from Dynamic Risk"));

            _transactions.Add(1, new TurboBudgetMockTransaction(1, new DateTime(2015, 3, 6), "Paycheque", _accounts[1], _accounts[3], 2500));
            _transactions.Add(2, new TurboBudgetMockTransaction(2, new DateTime(2015, 3, 6), "Paycheque", _accounts[3], _accounts[1], -2500));

            _transactions.Add(3, new TurboBudgetMockTransaction(3, new DateTime(2015, 3, 15), "Transfer to Automatic Account", _accounts[1], _accounts[2], 1304));
            _transactions.Add(4, new TurboBudgetMockTransaction(4, new DateTime(2015, 3, 15), "Transfer to Automatic Account", _accounts[2], _accounts[1], -1304));
        }

        #endregion
        #region ITurboBudgetRepository Properties

        IQueryable<ITurboBudgetAccount> ITurboBudgetRepository.Accounts
        {
            get { return _accounts.Values.AsQueryable(); }
        }

        IQueryable<ITurboBudgetTransaction> ITurboBudgetRepository.Transactions
        {
            get { return _transactions.Values.AsQueryable(); }
        }

        #endregion
        #region ITurboBudgetRepository Methods

        ITurboBudgetAccount ITurboBudgetRepository.CreateAccount()
        {
            return new TurboBudgetMockAccount();
        }

        ITurboBudgetTransaction ITurboBudgetRepository.CreateTransaction()
        {
            return new TurboBudgetMockTransaction();
        }

        void ITurboBudgetRepository.AddAccount(ITurboBudgetAccount account)
        {
            if (account != null)
            {
                _accounts.Add(account.ID, account);
            }
        }

        void ITurboBudgetRepository.AddTransaction(ITurboBudgetTransaction transaction)
        {
            if (transaction != null)
            {
                _transactions.Add(transaction.ID, transaction);
            }
        }

        #endregion
    }
}
