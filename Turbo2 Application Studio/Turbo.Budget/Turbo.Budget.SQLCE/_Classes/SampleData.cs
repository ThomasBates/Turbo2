using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.Entity;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    public class SampleData : DropCreateDatabaseIfModelChanges<TurboBudgetDbContext>
    {
        protected override void Seed(TurboBudgetDbContext context)
        {
            var accounts = new List<BudgetAccount>
            {
                new BudgetAccount() { ID = "DEPOSIT", Caption = "Deposit", Description = "Main Deposit Account"},
                new BudgetAccount() { ID = "AUTOMATIC", Caption = "Automatic", Description = "Automatic Debits and Cheques"},
                new BudgetAccount() { ID = "PAYCHEQUE", Caption = "Paycheque", Description = "Paycheque from Dynamic Risk"},
            };
            accounts.ForEach(a => context.Accounts.Add(a));

            new List<BudgetTransaction>
            {
                new BudgetTransaction() { TransactionId = 1, TimeStamp = new DateTime(2015, 3, 6), FromAccount = accounts.Single(a => a.ID == "PAYCHEQUE"), ToAccount = accounts.Single(a => a.ID == "DEPOSIT"), Amount = 2500, Description = "Paycheque"},
                new BudgetTransaction() { TransactionId = 2, TimeStamp = new DateTime(2015, 3, 15), FromAccount = accounts.Single(a => a.ID == "DEPOSIT"), ToAccount = accounts.Single(a => a.ID == "AUTOMATIC"), Amount = 1304, Description = "Transfer to Automatic Account"},
            }.ForEach(t => context.Transactions.Add(t));
        }
    }
}
