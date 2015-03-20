using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    public class TurboBudgetDbContext : DbContext
    {
        public DbSet<BudgetAccount> Accounts { get; set; }
        public DbSet<BudgetTransaction> Transactions { get; set; }
    }
}
