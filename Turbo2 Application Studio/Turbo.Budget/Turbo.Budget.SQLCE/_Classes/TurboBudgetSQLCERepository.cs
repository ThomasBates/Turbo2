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

        TurboBudgetEntities _entities = new TurboBudgetEntities();

        #endregion
        #region Constructors

        public TurboBudgetSQLCERepository()
        {
            Database.SetInitializer<TurboBudgetEntities>(new SampleData());
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
                return _entities.Accounts;
            }
        }

        IQueryable<ITurboBudgetTransaction> ITurboBudgetRepository.Transactions
        {
            get
            {
                return _entities.Transactions;
            }
        }

        #endregion
        #region ITurboBudgetRepository Methods

        ITurboBudgetAccount ITurboBudgetRepository.CreateAccount()
        {
            return new TurboBudgetAccount();
        }

        ITurboBudgetTransaction ITurboBudgetRepository.CreateTransaction()
        {
            return new TurboBudgetTransaction();
        }

        void ITurboBudgetRepository.AddAccount(ITurboBudgetAccount account)
        {
            if (account != null)
            {
                _entities.Accounts.Add(account);
            }
        }

        void ITurboBudgetRepository.AddTransaction(ITurboBudgetTransaction transaction)
        {
            if (transaction != null)
            {
                _entities.Transactions.Add(transaction);
            }
        }

        #endregion
    }
}
