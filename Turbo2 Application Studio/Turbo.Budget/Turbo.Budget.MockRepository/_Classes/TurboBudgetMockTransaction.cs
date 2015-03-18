using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.MockRepository
{
    class TurboBudgetMockTransaction : ITurboBudgetTransaction
    {
        public TurboBudgetMockTransaction()
        {
        }

        public TurboBudgetMockTransaction(int id, DateTime timeStamp, string description, ITurboBudgetAccount account, ITurboBudgetAccount otherAccount, decimal amount)
        {
            ((ITurboBudgetTransaction)this).ID = id;
            ((ITurboBudgetTransaction)this).TimeStamp = timeStamp;
            ((ITurboBudgetTransaction)this).Description = description;
            ((ITurboBudgetTransaction)this).Account = account;
            ((ITurboBudgetTransaction)this).OtherAccount = otherAccount;
            ((ITurboBudgetTransaction)this).Amount = amount;
        }

        int ITurboBudgetTransaction.ID { get; set; }
        DateTime ITurboBudgetTransaction.TimeStamp { get; set; }
        string ITurboBudgetTransaction.Description { get; set; }
        ITurboBudgetAccount ITurboBudgetTransaction.Account { get; set; }
        ITurboBudgetAccount ITurboBudgetTransaction.OtherAccount { get; set; }
        decimal ITurboBudgetTransaction.Amount { get; set; }
    }
}
