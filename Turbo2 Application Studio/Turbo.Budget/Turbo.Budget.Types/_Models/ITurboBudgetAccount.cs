using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Turbo.Budget.Types
{
    public interface ITurboBudgetAccount
    {
        string Id { get; set; }

        string Caption { get; set; }
        string Description { get; set; }

        List<ITurboBudgetTransaction> Transactions { get; }
    }
}
