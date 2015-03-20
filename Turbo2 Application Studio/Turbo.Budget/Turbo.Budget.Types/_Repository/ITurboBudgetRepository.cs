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

        ITurboBudgetAccount CreateAccount(
            string id, 
            string caption, 
            string description);
        
        ITurboBudgetTransaction CreateTransaction(
            int order,
            DateTime timeStamp, 
            string fromAccountId,
            string toAccountId, 
            decimal amount, 
            string description);

        void AddAccount(ITurboBudgetAccount account);
        void AddTransaction(ITurboBudgetTransaction transaction);

        IQueryable<ITurboBudgetTransaction> GetAcccountTransactions(ITurboBudgetAccount account);
    }
}
