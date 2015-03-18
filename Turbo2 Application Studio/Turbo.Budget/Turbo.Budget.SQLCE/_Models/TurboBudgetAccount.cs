using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    class TurboBudgetAccount : ITurboBudgetAccount
    {
        List<ITurboBudgetTransaction> _transactions = new List<ITurboBudgetTransaction>();

        public int AccountId { get; set; }
        public string Name { get; set; }
        public string Caption { get; set; }
        public string Description { get; set; }
        public List<ITurboBudgetTransaction> Transactions
        {
            get { return _transactions; }
        }
    }
}
