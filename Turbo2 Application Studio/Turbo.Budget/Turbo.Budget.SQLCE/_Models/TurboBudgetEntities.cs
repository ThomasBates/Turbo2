using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    public class TurboBudgetEntities : DbContext
    {
        public DbSet<ITurboBudgetAccount> Accounts { get; set; }
        public DbSet<ITurboBudgetTransaction> Transactions { get; set; }
    }
}
