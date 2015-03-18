using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.Entity;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    public class SampleData : DropCreateDatabaseIfModelChanges<TurboBudgetEntities>
    {
        protected override void Seed(TurboBudgetEntities context)
        {
            var accounts = new List<ITurboBudgetAccount>
            {
                new TurboBudgetAccount() { AccountId = 1, Name = "DEPOSIT", Caption = "Deposit", Description = "Main Deposit Account"},
                new TurboBudgetAccount() { AccountId = 2, Name = "AUTOMATIC", Caption = "Automatic", Description = "Automatic Debits and Cheques"},
                new TurboBudgetAccount() { AccountId = 3, Name = "PAYCHEQUE", Caption = "Paycheque", Description = "Paycheque from Dynamic Risk"},
            };

            new List<ITurboBudgetTransaction>
            {
                new TurboBudgetTransaction() { TransactionId = 1, TimeStamp = new DateTime(2015, 3, 6), FromAccount = accounts.Single(a => a.Name == "PAYCHEQUE"), ToAccount = accounts.Single(a => a.Name == "DEPOSIT"), Amount = 2500, Description = "Paycheque"},
                new TurboBudgetTransaction() { TransactionId = 2, TimeStamp = new DateTime(2015, 3, 15), FromAccount = accounts.Single(a => a.Name == "DEPOSIT"), ToAccount = accounts.Single(a => a.Name == "AUTOMATIC"), Amount = 1304, Description = "Transfer to Automatic Account"},
            }.ForEach(t => context.Transactions.Add(t));
        }
    }
}
