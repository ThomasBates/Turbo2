using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    public class TurboBudgetSQLCERepository : ITurboBudgetRepository
    {
        #region Private Members

        TurboBudgetDbContext _dbContext = new TurboBudgetDbContext();

        #endregion
        #region Constructors

        public TurboBudgetSQLCERepository()
        {
            Database.SetInitializer<TurboBudgetDbContext>(new SampleData());
            //AreaRegistration.RegisterAllAreas();
            //RegisterGlobalFilters(GlobalFilters.Filters);
            //RegisterRoutes(RouteTable.Routes);
        }

        #endregion
        #region ITurboBudgetRepository Properties

        IQueryable<ITurboBudgetAccount> ITurboBudgetRepository.Accounts
        {
            get
            {
                return _dbContext.Accounts;
            }
        }

        IQueryable<ITurboBudgetTransaction> ITurboBudgetRepository.Transactions
        {
            get
            {
                return _dbContext.Transactions;
            }
        }

        #endregion
        #region ITurboBudgetRepository Methods

        ITurboBudgetAccount ITurboBudgetRepository.CreateAccount(
            string name, 
            string caption, 
            string description)
        {
            return new BudgetAccount()
            {
                ID = name,
                Caption = caption,
                Description = description
            };
        }

        ITurboBudgetTransaction ITurboBudgetRepository.CreateTransaction(
            DateTime timeStamp, 
            ITurboBudgetAccount fromAccount, 
            ITurboBudgetAccount toAccount, 
            decimal amount, 
            string description)
        {
            return new BudgetTransaction()
            {
                TimeStamp = timeStamp,
                FromAccountId = fromAccount.AccountId,
                FromAccount = fromAccount,
                ToAccountId = toAccount.AccountId,
                ToAccount = toAccount,
                Amount = amount,
                Description = description
            };
        }

        void ITurboBudgetRepository.AddAccount(ITurboBudgetAccount account)
        {
            if (account is BudgetAccount)
            {
                _dbContext.Accounts.Add(account as BudgetAccount);
                _dbContext.SaveChanges();
            }
        }

        void ITurboBudgetRepository.AddTransaction(ITurboBudgetTransaction transaction)
        {
            if (transaction is BudgetTransaction)
            {
                _dbContext.Transactions.Add(transaction as BudgetTransaction);
                _dbContext.SaveChanges();
            }
        }

        #endregion
    }
}
