using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Turbo.Budget.Types
{
    public interface ITurboBudgetRepository
    {
        IQueryable<ITurboBudgetAccount> Accounts { get; }

        IQueryable<ITurboBudgetTransaction> Transactions { get; }

        ITurboBudgetAccount CreateAccount();

        ITurboBudgetTransaction CreateTransaction();

        void AddAccount(ITurboBudgetAccount account);

        void AddTransaction(ITurboBudgetTransaction transaction);
    }
}
